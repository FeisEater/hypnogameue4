// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionFreeze.h"
#include "AICharacter.h"

void HActionFreeze::RunAction()
{
	if (!m_owner->IsWaiting())
	{
		m_owner->Wait(m_waitTime);
		Consume();
	}
}

