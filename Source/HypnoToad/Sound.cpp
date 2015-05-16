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
		if (FVector::Dist(sound->Origin, ActorItr->GetActorLocation()) < sound->Range)
			ActorItr->HearSound(sound);
	}
	//Show sound notifier in gui through event call
	APlayerController* plrController = *(world->GetPlayerControllerIterator());
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)plrController->GetCharacter();
	plr->ShowSoundNotifier(sound);
}