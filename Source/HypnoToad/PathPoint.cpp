// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "PathPoint.h"
#include "Engine.h"
#include "AICharacter.h"

// Sets default values
APathPoint::APathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MyCollisionComp = CreateDefaultSubobject<USphereComponent>(FName(TEXT("CollisionComponent")));
	MyCollisionComp->RelativeLocation = FVector::ZeroVector;
	MyCollisionComp->SetSphereRadius(20.0f);
	MyCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APathPoint::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APathPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APathPoint::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AAICharacter::StaticClass()))
	{
		AAICharacter* ai = (AAICharacter*)OtherActor;
		ai->WaitAndHeadToNextPoint(this);
	}
}