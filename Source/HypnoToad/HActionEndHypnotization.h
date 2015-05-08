// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionEndHypnotization : public HAction
{
public:
	HActionEndHypnotization(AAICharacter* owner, bool overrideAttack = true) : HAction(owner, overrideAttack)
	{
		m_workWhileHypnotized = true;
	}
	FString GetMenuName() override { return "End hypnotization"; }
	HAction* CreateAction() override { return new HActionEndHypnotization(m_owner, m_overrideAttackState); }

protected:
	virtual void RunAction() override;
};
