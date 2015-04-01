// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "Sticker.h"


// Sets default values
ASticker::ASticker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASticker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASticker::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

