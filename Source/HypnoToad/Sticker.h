// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Sticker.generated.h"

/** Sticker actor, that ai characters can see. Just Unreal's standard decal actor with box component */

UCLASS()
class HYPNOTOAD_API ASticker : public ADecalActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASticker(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** box collider */
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class UBoxComponent* BoxCollider;
	
	//Compiler complained about the 4 following functions
	class UMaterialInstanceDynamic* CreateDynamicMaterialInstance()
	{
		return NULL;
	}

	void PostEditMove(bool bFinished) {}
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {}
	void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) {}
};
