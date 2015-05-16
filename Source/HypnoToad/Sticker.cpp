// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "Sticker.h"


// Sets default values
ASticker::ASticker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachTo(RootComponent);
	BoxCollider->InitBoxExtent(FVector(1, 1, 1));

	//Box collision needed so it is visible to ai characters
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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

