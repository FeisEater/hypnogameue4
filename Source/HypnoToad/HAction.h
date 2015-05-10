// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound.h"

/**
 * 
 */
class AAICharacter;

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

	void Execute();
	virtual FString GetMenuName() = 0;
	virtual HAction* CreateAction() = 0;
	virtual void CollectParameters();
	virtual void SetActorParameter(AActor* actor);
	virtual void SetVectorParameter(TSharedPtr<FVector> vector);
	virtual void SetSoundParameter(USound* sound);
	virtual void SetStringParameter(FString string);

protected:
	virtual void RunAction() {}
	void Consume();

	AAICharacter* m_owner;
	bool m_overrideAttackState;
	bool m_workWhileHypnotized;
	//HConsumptionMethod m_conMethod;
	float m_actionCount;
};
