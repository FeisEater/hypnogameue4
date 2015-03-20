// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PathPoint.h"
#include "AICharacter.generated.h"

UCLASS()
class HYPNOTOAD_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	APathPoint* StartPPoint;

	void WaitAndHeadToNextPoint(APathPoint* NextPoint);

private:
	APathPoint* PPoint;
	float waitTime;
};
