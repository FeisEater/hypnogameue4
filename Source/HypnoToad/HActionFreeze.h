// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"
/**
 * 
 */
class HYPNOTOAD_API HActionFreeze : public HAction
{
public:
	HActionFreeze(AAICharacter* owner) : HAction(owner) {}

	virtual void Execute() override;
};
