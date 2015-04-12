// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"

/**
 * 
 */
class HYPNOTOAD_API HTriggerSawPlayerHypnotizing : public HTrigger
{
public:
	HTriggerSawPlayerHypnotizing(AAICharacter* owner) : HTrigger(owner) {}

protected:
	virtual bool IsTriggered() override;
};
