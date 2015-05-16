// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionShoot.h"
#include "AICharacter.h"

void HActionShoot::RunAction()
{
	//Use timer for consumption.
	if (m_owner->GetWorldTimerManager().GetTimerRemaining(m_timer) <= 0)
	{
		m_owner->Shoot();
		m_owner->GetWorldTimerManager().SetTimer(m_timer, 1, false);
		Consume();
	}
}