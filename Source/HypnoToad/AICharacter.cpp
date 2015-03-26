// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PPoint = StartPPoint;
	DesiredRotation = GetActorRotation();
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	APlayerController* plr = *GetWorld()->GetPlayerControllerIterator();
	FVector plrLoc = plr->GetCharacter()->GetActorLocation();
	FVector diff = plrLoc - GetActorLocation();
	diff.Normalize();
	float dot = FVector::DotProduct(GetActorRotation().Vector(), diff);
	if (dot > 0.7f)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FHitResult Hit;

		if (GetWorld()->LineTraceSingle(Hit, GetActorLocation(), plrLoc, ECC_Pawn, Params) && Hit.Actor.Get() == plr->GetCharacter())
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("I see you"));
		}
	}

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