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

protected:
	virtual void RunAction() override;

private:
	AActor* m_target;
};
