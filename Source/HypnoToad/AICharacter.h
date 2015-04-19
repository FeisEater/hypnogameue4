// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PathPoint.h"
#include "HTrigger.h"
#include "HypnoToadCharacter.h"
#include "HSound.h"
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

	bool CanSee(AActor* actor);
	void Attack(AActor* actor);
	bool IsAttacking();
	void Shoot();

	APathPoint* GetNextPPoint();
	void SetNextPPoint(APathPoint* pp);
	void WaitAndHeadToNextPoint(APathPoint* NextPoint);

	void ActivateConversation(AHypnoToadCharacter* plr);
	void EndConversation();

	void Hypnotize(AHypnoToadCharacter* plr);
	void EndHypnotization();
	bool IsHypnotized();
	void StayStillWhileHypnotized();
	void HearSound(TSharedPtr<HSound> sound);
	TSharedPtr<HSound> HeardSound(TSharedPtr<HSound> sound);

	FRotator DesiredRotation;

	float waitTime;	//move to private

	TArray<HTrigger*> GetTriggers()
	{
		return m_availableTriggers;
	}
	void PrepareTriggerViaIndex(int32 index);
	void AttachPendingTrigger();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	APathPoint* PPoint;
	TArray<HTrigger*> triggers;
	bool m_havingConversation;
	AHypnoToadCharacter* m_hypnotizedBy;
	bool m_followsHypnotizer;
	float m_rebuildPathTime;
	float m_rateOfFire;
	float m_timeToCare;
	bool m_attacking;
	AActor* m_currentEnemy;
	FVector m_lastEnemyPosition;
	float m_scanPosition;
	TArray<TSharedPtr<HSound>> m_heardSounds;
	TArray<HTrigger*> m_availableTriggers;
	HTrigger* m_pendingTrigger;
};
