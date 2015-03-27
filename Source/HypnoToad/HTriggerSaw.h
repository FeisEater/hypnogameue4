// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTrigger.h"
/**
 * 
 */

class HYPNOTOAD_API HTriggerSaw : public HTrigger
{
public:
	HTriggerSaw(AAICharacter* owner, UClass* cl) : HTrigger(owner), m_class(cl) {}
	virtual bool IsTriggered() override;

	bool CanSee(AActor* actor);

private:
	UClass* m_class;

};
