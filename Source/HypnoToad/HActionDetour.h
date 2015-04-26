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
	FString GetMenuName() override { return "Detour to..."; }
	HAction* CreateAction() override { return new HActionDetour(m_owner, m_targetPosition); }
	void CollectParameters() override;
	void SetActorParameter(AActor* actor) override
	{
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
	TSharedPtr<FVector> m_targetPosition;
	class APathPoint* m_detourPoint;
};
