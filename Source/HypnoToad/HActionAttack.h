// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionAttack : public HAction
{
public:
	HActionAttack(AAICharacter* owner, class AActor* target, bool overrideAttack = true) : HAction(owner, overrideAttack), m_target(target) {}
	FString GetMenuName() override { return "Attack..."; }
	HAction* CreateAction() override { return new HActionAttack(m_owner, m_target); }
	void CollectParameters() override;

protected:
	virtual void RunAction() override;

private:
	AActor* m_target;
};
