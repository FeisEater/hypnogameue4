// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerSawHypnotizedNpc : public HTrigger
{
public:
	HTriggerSawHypnotizedNpc(AAICharacter* owner) : HTrigger(owner) {}
	TSharedPtr<FVector> GetHypnotizedNpcLocation();
protected:
	virtual bool IsTriggered() override;
	FVector m_hypnotizedNpcLocation;
};