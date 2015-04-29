// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "Sound.h"
#include "AICharacter.h"

USound::USound(const FObjectInitializer& ObjectInitializer)
{

}

void USound::BroadCastSound(UWorld* world, USound* sound)
{
	for (TActorIterator<AAICharacter> ActorItr(world); ActorItr; ++ActorItr)
	{
		if ((sound->Origin - ActorItr->GetActorLocation()).Size() < sound->Range)
			ActorItr->HearSound(sound);
	}
	APlayerController* plrController = *(world->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowSoundNotifier(sound);
}