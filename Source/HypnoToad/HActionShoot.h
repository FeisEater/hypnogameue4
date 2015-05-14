// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionShoot : public HAction
{
public:
	HActionShoot(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_actionCount = 3;
	}
	FString GetMenuName() override { return "Shoot once" + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionShoot(m_owner, m_overrideAttackState); }

protected:
	virtual void RunAction() override;

private:
	FTimerHandle m_timer;
};
