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
	RootComponent = MyCollisionComp;
	MyCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APathPoint::OnOverlapBegin);
	
	Overridable = false;
}

// Called when the game starts or when spawned
void APathPoint::BeginPlay()
{
	Super::BeginPlay();
	m_gotVisited = false;
}

// Called every frame
void APathPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APathPoint::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AAICharacter::StaticClass()))
		return;
	AAICharacter* ai = (AAICharacter*)OtherActor;
	if (ai->GetNextPPoint() == this)
	{
		ai->WaitAndHeadToNextPoint(this);
		m_gotVisited = true;
	}
}

bool APathPoint::GotVisited()
{
	return m_gotVisited;
}