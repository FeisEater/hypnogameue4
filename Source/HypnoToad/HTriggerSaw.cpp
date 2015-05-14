// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "HTriggerSaw.h"
#include "AICharacter.h"

bool HTriggerSaw::IsTriggered()
{
	for (TActorIterator<AActor> ActorItr(m_owner->GetWorld()); ActorItr; ++ActorItr)
	{
		if ((*ActorItr) == m_actor && m_owner->CanSee(*ActorItr))
		{
			return true;
		}
	}
	return false;
}

void HTriggerSaw::CollectParameters()
{
	APlayerController* plrController = *(m_owner->GetWorld()->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	TArray<AActor*> actorsToSee = plr->GetStickers();
	actorsToSee += plr->GetNpcAttackTargets();
	plr->ShowActorParameterGui(actorsToSee);
}

void HTriggerSaw::SetActorParameter(AActor* actor)
{
	m_actor = actor;
	HTrigger::SetActorParameter(actor);
}