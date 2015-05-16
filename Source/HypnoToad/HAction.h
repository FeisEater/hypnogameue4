// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound.h"

/**
 * Action to invoke when somwhow triggered. Makes owning npc to change behaviour.
 */
class AAICharacter;

//Was thinking about adding different consumption methods, whether its through discrete count or timing.
/*enum class HConsumptionMethod
{
	Indefinate,
	Count,
	Time
};*/

class HYPNOTOAD_API HAction
{
public:
	HAction(AAICharacter* owner, bool overrideAttack) : m_owner(owner), m_overrideAttackState(overrideAttack), m_workWhileHypnotized(false) {}
	virtual ~HAction() {}

	/** Public function to invoke on trigger. */
	void Execute();
	/** Name of the action to show in gui. */
	virtual FString GetMenuName();
	/** Factory function to create new action from this template. */
	virtual HAction* CreateAction() = 0;
	/** Handle parameter collecting for action. */
	virtual void CollectParameters();
	/** Pass actor parameter to this action. */
	virtual void SetActorParameter(AActor* actor);
	/** Pass vector parameter to this action. */
	virtual void SetVectorParameter(TSharedPtr<FVector> vector);
	/** Pass sound parameter to this action. */
	virtual void SetSoundParameter(USound* sound);
	/** Pass string parameter to this action. */
	virtual void SetStringParameter(FString string);

protected:
	/** Overridable function that invokes correct behaviour. */
	virtual void RunAction() {}
	/** Decrements counter for this function. */
	void Consume();

	/** Character that owns this action and is influenced by this action. */
	AAICharacter* m_owner;
	/** If true, action is run even if npc is currently in attack state */
	bool m_overrideAttackState;
	/** If true, action is run even if npc is currently in sleep state */
	bool m_workWhileHypnotized;
	//HConsumptionMethod m_conMethod;
	/** Counter, which deletes this action and trigger when reached zero */
	float m_actionCount;
	/** Shortcut function to get player */
	class AHypnoToadCharacter* GetPlayer();

private:
	/** Find trigger that owns this action. */
	class HTrigger* GetTrigger();

	/** Run default parameter collection routine if not overloaded. */
	void DefaultParameterCollection();
};
