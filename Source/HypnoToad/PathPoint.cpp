// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "PathPoint.h"


// Sets default values
APathPoint::APathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MyCollisionComp = CreateDefaultSubobject<USphereComponent>(FName(TEXT("CollisionComponent")));
	MyCollisionComp->RelativeLocation = FVector::ZeroVector;
	MyCollisionComp->SetSphereRadius(20.0f);

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

