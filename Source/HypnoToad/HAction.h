// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class AAICharacter;

class HYPNOTOAD_API HAction
{
public:
	HAction(AAICharacter* owner) : m_owner(owner) {}

	virtual void Execute() {}

protected:
	AAICharacter* m_owner;
};
