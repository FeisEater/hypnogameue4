// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Sticker.generated.h"

UCLASS()
class HYPNOTOAD_API ASticker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASticker();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** box collider */
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class USphereComponent* Sphere1;
	
};
