// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAction.h"

/**
 * Detour to some point from default path
 */
class HYPNOTOAD_API HActionDetour : public HAction
{
public:
	HActionDetour(AAICharacter* owner, TSharedPtr<FVector> target, float maxDist = 3000, float actionCount = 5, bool overrideAttack = false) : 
		HAction(owner, overrideAttack), m_targetPosition(target), m_maxDistance(maxDist)
	{
		m_detourPoint = NULL;
		m_actionCount = actionCount;
	}
	FString GetMenuName() override { return "Detour to..." + HAction::GetMenuName(); }
	HAction* CreateAction() override { return new HActionDetour(m_owner, m_targetPosition, m_maxDistance, m_actionCount, m_overrideAttackState); }
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override
	{
		//Get actor parameter from gui.
		m_targetPosition = TSharedPtr<FVector>(new FVector(actor->GetActorLocation()));
		HAction::SetActorParameter(actor);
	}
	void SetVectorParameter(TSharedPtr<FVector> vector) override
	{
		m_targetPosition = vector;
		HAction::SetVectorParameter(vector);
	}

protected:
	virtual void RunAction() override;

private:
	/** Location to which npc will detour to. */
	TSharedPtr<FVector> m_targetPosition;
	/** Temporary pathpoint object at target position. */
	class APathPoint* m_detourPoint;
	/** Maximum distance at which npc will detour. */
	float m_maxDistance;
};
