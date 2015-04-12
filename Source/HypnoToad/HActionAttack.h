// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionAttack : public HAction
{
public:
	HActionAttack(AAICharacter* owner, class AActor* target) : HAction(owner), m_target(target) {}
	virtual void Execute() override;

private:
	AActor* m_target;
};
