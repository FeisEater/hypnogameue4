// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionWakeNpc : public HAction
{
public:
	HActionWakeNpc(AAICharacter* owner) : HAction(owner)
	{
		m_detourPoint = NULL;
	}

	virtual void Execute() override;

private:
	bool CanSee(AAICharacter* actor);
	class APathPoint* m_detourPoint;
};
