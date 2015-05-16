// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * Sets enemy
 */
class HYPNOTOAD_API HActionAttack : public HAction
{
public:
	HActionAttack(AAICharacter* owner, class AActor* target, bool overrideAttack = true) : HAction(owner, overrideAttack), m_target(target)
	{
		m_actionCount = 1;
	}
	FString GetMenuName() override;
	HAction* CreateAction() override { return new HActionAttack(m_owner, m_target, m_overrideAttackState); }
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override;

protected:
	virtual void RunAction() override;

private:
	/** Actor to set as enemy */
	AActor* m_target;
};
