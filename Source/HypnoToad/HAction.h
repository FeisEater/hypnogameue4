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

protected:
	virtual void RunAction() {}

	AAICharacter* m_owner;
	bool m_overrideAttackState;
};
