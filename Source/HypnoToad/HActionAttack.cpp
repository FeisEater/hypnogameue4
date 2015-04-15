// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HActionAttack.h"
#include "AICharacter.h"

void HActionAttack::RunAction()
{
	m_owner->Attack(m_target);
}