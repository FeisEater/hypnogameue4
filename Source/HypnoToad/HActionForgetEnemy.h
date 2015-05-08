// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionForgetEnemy : public HAction
{
public:
	HActionForgetEnemy(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack) {}
	FString GetMenuName() override { return "Forget enemy"; }
	HAction* CreateAction() override { return new HActionForgetEnemy(m_owner, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
