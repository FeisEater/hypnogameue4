// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PathPoint.generated.h"

UCLASS()
class HYPNOTOAD_API APathPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathPoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Next pathpoint in patrolling ai character's path
	UPROPERTY(EditAnywhere)
	AActor* NextPPoint;
	
	//Time to wait when reached this pathpoint
	UPROPERTY(EditAnywhere)
	float waitTime;

	UPROPERTY()
	USphereComponent* MyCollisionComp;

	/** Called when something enters the sphere component */
	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool GotVisited();
	
	/** True if pathpoint is temporary and injected into the path dynamically.
		When the pathpoint is destroyed and path restored, the following pathpoints are skipped if they're overridable as well
	*/
	bool Overridable;

	/** True if pathpoint was placed by player. */
	bool PlacedByPlayer;

private:
	/** Keeps track if pathpoint was visited since pathpoints creation. */
	bool m_gotVisited;
};
