// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionFreeze.h"
#include "AICharacter.h"

void HActionFreeze::RunAction()
{
	if (m_owner->waitTime <= 0)
	{
		m_owner->waitTime = 1;
		m_owner->GetController()->StopMovement();
		Consume();
	}
}

