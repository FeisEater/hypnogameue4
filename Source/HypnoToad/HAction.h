// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class AAICharacter;

class HYPNOTOAD_API HAction
{
public:
	HAction(AAICharacter* owner, bool overrideAttack) : m_owner(owner), m_overrideAttackState(overrideAttack) {}

	void Execute();
	virtual FString GetMenuName() = 0;
	virtual HAction* CreateAction() = 0;
	virtual void CollectParameters();

protected:
	virtual void RunAction() {}

	AAICharacter* m_owner;
	bool m_overrideAttackState;
};
