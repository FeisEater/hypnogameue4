// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"
/**
 * 
 */
class HYPNOTOAD_API HActionIgnore : public HAction
{
public:
	HActionIgnore(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack) {}
	FString GetMenuName() override { return "Ignore"; }
	HAction* CreateAction() override { return new HActionIgnore(m_owner); }

protected:
	virtual void RunAction() override;
};
