// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PathPoint.h"
#include "HTrigger.h"
#include "HypnoToadCharacter.h"
#include "Sound.h"
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

	// Called when the game ends
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	APathPoint* StartPPoint;

	bool CanSee(AActor* actor);
	void Attack(AActor* actor);
	bool IsAttacking();
	
	UFUNCTION(BlueprintCallable, Category = AI)
	bool HasEnemy();

	UFUNCTION(BlueprintCallable, Category = AI)
	AActor* GetEnemy();

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsInfluenced();

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsMaxedInfluenced();

	void StopAttack(bool forgetEnemy);
	void Shoot();
	void Hurt(AAICharacter* victim);

	APathPoint* GetNextPPoint();
	void SetNextPPoint(APathPoint* pp);
	void WaitAndHeadToNextPoint(APathPoint* NextPoint);

	void ActivateConversation(AHypnoToadCharacter* plr);
	void EndConversation();

	void Hypnotize(AHypnoToadCharacter* plr, bool followPlayer);
	void EndHypnotization();

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsHypnotized();
	
	void StayStillWhileHypnotized();
	void HearSound(USound* sound);
	USound* HeardSound(USound* sound);

	FRotator DesiredRotation;

	float waitTime;	//move to private

	TArray<HTrigger*> GetTriggers()
	{
		return m_availableTriggers;
	}
	TArray<HAction*> GetActions();
	TArray<HTrigger*>& GetActiveTriggers()
	{
		return triggers;
	}
	void RemoveActiveTrigger(HTrigger* trigger);
	void PrepareTriggerViaIndex(int32 index);
	void AttachPendingTrigger();
	HTrigger* GetPendingTrigger()
	{
		return m_pendingTrigger;
	}
	void PrepareActiveTriggerViaIndex(int32 index);

	void PrepareActionViaIndex(int32 index);
	HAction* GetPendingAction()
	{
		return m_pendingAction;
	}

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool Discovered;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	int32 MaxTriggers;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool Armed;

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsDead();

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
	TArray<USound*> m_heardSounds;
	TArray<HTrigger*> m_availableTriggers;
	HTrigger* m_pendingTrigger;
	TArray<HAction*> m_availableLightActions;
	TArray<HAction*> m_availableHeavyActions;
	HAction* m_pendingAction;
	int32 m_health;
	TArray<HTrigger*> m_triggersToRemove;
};
