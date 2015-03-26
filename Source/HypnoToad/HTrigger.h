// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class AAICharacter;

class HYPNOTOAD_API HTrigger
{
public:
	HTrigger(AAICharacter* owner) : m_owner(owner) {}

	virtual bool IsTriggered() { return false; }

protected:
	AAICharacter* m_owner;

};
