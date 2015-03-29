// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTrigger.h"

void HTrigger::Trigger()
{
	if (IsTriggered() && m_action)
		m_action->Execute();
}