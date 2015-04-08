// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PathPoint.h"
#include "HTrigger.h"
#include "HypnoToadCharacter.h"
#include "AICharacter.generated.h"

UCLASS()
class HYPNOTOAD_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	~AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	APathPoint* StartPPoint;

	APathPoint* GetNextPPoint();
	void SetNextPPoint(APathPoint* pp);
	void WaitAndHeadToNextPoint(APathPoint* NextPoint);

	void ActivateConversation(AHypnoToadCharacter* plr);
	void EndConversation();

	void Hypnotize(AHypnoToadCharacter* plr);
	void EndHypnotization();
	bool IsHypnotized();

	FRotator DesiredRotation;

	float waitTime;	//move to private

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	APathPoint* PPoint;
	TArray<HTrigger*> triggers;
	bool m_havingConversation;
	AHypnoToadCharacter* m_hypnotizedBy;
	float m_rebuildPathTime;
};
