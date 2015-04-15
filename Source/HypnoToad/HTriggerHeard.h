// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HTrigger.h"
#include "HSound.h"
/**
 * 
 */
class HYPNOTOAD_API HTriggerHeard : public HTrigger
{
public:
	HTriggerHeard(AAICharacter* owner, TSharedPtr<HSound> sound) : HTrigger(owner), m_sound(sound) {}

protected:
	virtual bool IsTriggered() override;

private:
	TSharedPtr<HSound> m_sound;
};
