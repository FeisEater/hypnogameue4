// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"

/**
 * 
 */
class HYPNOTOAD_API HActionDetour : public HAction
{
public:
	HActionDetour(AAICharacter* owner, TSharedPtr<FVector> target, bool overrideAttack = false) : HAction(owner, overrideAttack), m_targetPosition(target)
	{
		m_detourPoint = NULL;
	}

protected:
	virtual void RunAction() override;

private:
	TSharedPtr<FVector> m_targetPosition;
	class APathPoint* m_detourPoint;
};
