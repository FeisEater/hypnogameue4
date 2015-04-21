// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
/**
 * 
 */
class HYPNOTOAD_API HActionFreeze : public HAction
{
public:
	HActionFreeze(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack) {}
	FString GetMenuName() override { return "Freeze in place"; }
	HAction* CreateAction() override { return new HActionFreeze(m_owner); }

protected:
	virtual void RunAction() override;
};
