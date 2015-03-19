// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "Engine.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::HitPPoint);

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PPoint = StartPPoint;
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UNavigationSystem::SimpleMoveToActor(Controller, PPoint);
	//UNavigationSystem::
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Player->);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AAICharacter::HitPPoint(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Now what?");
	PPoint = (APathPoint*)PPoint->NextPPoint;
}