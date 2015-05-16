// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PathPoint.h"
#include "HTrigger.h"
#include "HypnoToadCharacter.h"
#include "Sound.h"
#include "AICharacter.generated.h"

/*
	Npc
*/

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

	/** Starting path point */
	UPROPERTY(EditAnywhere, Category = AI)
	APathPoint* StartPPoint;

	bool CanSee(AActor* actor);
	void SetEnemy(AActor* actor);
	bool IsAttacking();
	
	UFUNCTION(BlueprintCallable, Category = AI)
	bool HasEnemy();

	UFUNCTION(BlueprintCallable, Category = AI)
	AActor* GetEnemy();

	/** True if set of trigger-action pairs are not default ie. character was influenced by player */
	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsInfluenced();

	/** True if character can't be hypnotized further */
	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsMaxedInfluenced();

	/** Leaves attack state. If forgetEnemy is true, forgets current target as enemy */
	void StopAttack(bool forgetEnemy);
	void Shoot();
	void Hurt(AAICharacter* victim);

	bool IsWaiting();
	void Wait(float time);

	APathPoint* GetNextPPoint();
	void SetNextPPoint(APathPoint* pp);
	/** Sets next path point in path then waits a bit */
	void WaitAndHeadToNextPoint(APathPoint* NextPoint);

	void ActivateConversation(AHypnoToadCharacter* plr);
	void EndConversation();

	/** Sleeps to provide further hypnotization possibilities. If followPlayer is True, is escorted by player */
	void Sleep(AHypnoToadCharacter* plr, bool followPlayer);
	void EndSleep();

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsAsleep();
	
	/** Stops getting escorted by player while asleep */
	void StayStillWhileAsleep();
	/** Put sound to buffer */
	void HearSound(USound* sound);
	/** Return buffered sound equivalent to given sound */
	USound* HeardSound(USound* sound);

	//Rotation towards which character will try to face to through interpolation
	FRotator DesiredRotation;

	TArray<HTrigger*> GetTriggers()
	{
		return m_availableTriggers;
	}
	TArray<HAction*> GetActions();
	TArray<HTrigger*>& GetActiveTriggers()
	{
		return m_triggers;
	}

	/** Mark trigger to be removed. */
	void RemoveActiveTrigger(HTrigger* trigger);
	/** Set trigger at given index as pending. */
	void PrepareTriggerViaIndex(int32 index);
	/** Create trigger-action pair from pending trigger and action, ie. hypnotize */
	void AttachPendingTrigger();

	HTrigger* GetPendingTrigger()
	{
		return m_pendingTrigger;
	}

	/** Set active trigger at given index as pending. */
	void PrepareActiveTriggerViaIndex(int32 index);
	/** Set action at given index as pending. */
	void PrepareActionViaIndex(int32 index);

	HAction* GetPendingAction()
	{
		return m_pendingAction;
	}

	/** Npc is discovered and its information can be seen */
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool Discovered;

	/** Maximum amount of triggers npc can contain */
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	int32 MaxTriggers;

	/** Npc is capable to attack */
	UPROPERTY(EditAnywhere, Category = "AI")
	bool Armed;

	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsDead();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Notify trigger consumption. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowTriggerConsumption(const FString& message);

private:
	/** Stop rendering gun mesh */
	void HideWeapon();
	/** Initialize npc with default triggers and actions */
	void SetupDefaultTriggersActions();
	/** Trigger all triggers */
	void RunTriggers();
	void ClearRemovedTriggers();
	/** Clear sound buffer */
	void ClearHeardSounds();
	void VisualizePath();
	/** Returns direction vector towards enemy, or straight ahead if no enemy. */
	FVector FaceEnemy();
	/** Modifies Dir to spread from gun inaccuracy */
	FVector ApplySpread(FVector Dir);
	void Ragdollize();
	/** Enter attack state */
	void Attack();

	//State ticks:
	void AttackTick(float DeltaTime);
	void PatrolTick(float DeltaTime);
	void WaitTick(float DeltaTime);
	void FollowTick(float DeltaTime);

	/** Pathpoint towards which npc is currently headed */
	APathPoint* m_ppoint;

	/** Currently active triggers */
	TArray<HTrigger*> m_triggers;
	/** True if currently having a conversation, even if asleep */
	bool m_havingConversation;
	/** Player hypnotizing npc */
	AHypnoToadCharacter* m_hypnotizedBy;
	/** If true, is escorted by player */
	bool m_followsHypnotizer;
	/** Navigation is buggy if path is recalculated every tick, instead delay it for some time */
	float m_rebuildPathTime;
	float m_rateOfFire;
	/** When this runs out, npc leaves attack state */
	float m_timeToCare;
	bool m_attacking;
	AActor* m_currentEnemy;
	/** Last seen enemy position */
	FVector m_lastEnemyPosition;
	float m_scanPosition;
	/** Heard sound buffer */
	TArray<USound*> m_heardSounds;
	/** Collection of template triggers with which npc can be hypnotized. */
	TArray<HTrigger*> m_availableTriggers;
	HTrigger* m_pendingTrigger;
	/** Collection of template action with which npc can be hypnotized through conversation. */
	TArray<HAction*> m_availableLightActions;
	/** Collection of template action with which npc can be hypnotized through sleep. */
	TArray<HAction*> m_availableHeavyActions;
	HAction* m_pendingAction;
	int32 m_health;
	/** Triggers to remove after main trigger loop */
	TArray<HTrigger*> m_triggersToRemove;
	/** Time to wait at path point */
	float m_waitTime;
};
