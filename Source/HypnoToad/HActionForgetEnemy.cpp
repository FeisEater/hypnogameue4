// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionForgetEnemy.h"
#include "AICharacter.h"

void HActionForgetEnemy::RunAction()
{
	m_owner->StopAttack(true);
}
