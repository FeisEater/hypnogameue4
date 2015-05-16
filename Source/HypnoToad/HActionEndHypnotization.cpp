// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionEndHypnotization.h"
#include "AICharacter.h"

void HActionEndHypnotization::RunAction()
{
	if (m_owner->IsAsleep())
	{
		m_owner->EndSleep();
		Consume();
	}
}