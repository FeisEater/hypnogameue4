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
	m_havingConversation = false;
	m_hypnotizedBy = NULL;

	HTrigger* t = new HTriggerSaw(this, ADecalActor::StaticClass());
	t->SetAction(new HActionFreeze(this));
	triggers.Add(t);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (HTrigger* t : triggers)
		t->Trigger();

	if (waitTime <= 0 && !m_havingConversation && !m_hypnotizedBy)
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
	else if (!m_hypnotizedBy)
	{
		waitTime -= DeltaTime;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
	}
	else if (m_hypnotizedBy)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, m_hypnotizedBy);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
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

void AAICharacter::ActivateConversation(AHypnoToadCharacter* plr)
{
	GetController()->StopMovement();
	m_havingConversation = true;
	
	FVector vec = plr->GetActorLocation() - GetActorLocation();
	vec.Normalize();
	DesiredRotation = vec.Rotation();
	vec = DesiredRotation.Euler();
	vec.X = 0;
	vec.Y = 0;
	DesiredRotation = FRotator::MakeFromEuler(vec);
}

void AAICharacter::EndConversation()
{
	m_havingConversation = false;
}

void AAICharacter::Hypnotize(AHypnoToadCharacter* plr)
{
	m_hypnotizedBy = plr;
}

void AAICharacter::EndHypnotization()
{
	m_hypnotizedBy = NULL;
}

bool AAICharacter::IsHypnotized()
{
	return m_hypnotizedBy != NULL;
}