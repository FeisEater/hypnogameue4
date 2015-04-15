// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionWakeNpc : public HAction
{
public:
	HActionWakeNpc(AAICharacter* owner, bool overrideAttack = false) : HAction(owner, overrideAttack)
	{
		m_detourPoint = NULL;
	}

protected:
	virtual void RunAction() override;

private:
	class APathPoint* m_detourPoint;
};
