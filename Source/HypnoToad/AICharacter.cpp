// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "HTriggerSaw.h"
#include "HActionFreeze.h"
#include "HypnoToadCharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AAICharacter::~AAICharacter()
{
	for (HTrigger* t : triggers)
		delete t;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PPoint = StartPPoint;
	DesiredRotation = GetActorRotation();

	HTrigger* t = new HTriggerSaw(this, AHypnoToadCharacter::StaticClass());
	t->SetAction(new HActionFreeze(this));
	triggers.Add(t);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (HTrigger* t : triggers)
		t->Trigger();

	if (waitTime <= 0)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, PPoint);
		UNavigationPath* path = UNavigationSystem::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PPoint);
		FVector prev = path->PathPoints[0];
		for (FVector v : path->PathPoints)
		{
			DrawDebugLine(GetWorld(), prev, v, FColor::Green);
			prev = v;
		}
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
	}
	else
	{
		waitTime -= DeltaTime;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AAICharacter::WaitAndHeadToNextPoint(APathPoint* PrevPoint)
{
	waitTime = PrevPoint->waitTime;
	PPoint = (APathPoint*)PrevPoint->NextPPoint;
	DesiredRotation = PrevPoint->GetActorRotation();
}