// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Sound.h"
#include "HypnoToadCharacter.generated.h"

class AAICharacter;
class HTrigger;

UCLASS(config=Game)
class AHypnoToadCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AHypnoToadCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Fetches names of triggers of the npc with whom player interracts currently */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	TArray<FString> GetNpcTriggerNames();

	/** Fetches names of actions of the npc with whom player interracts currently */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	TArray<FString> GetNpcActionNames();

	/** Attaches a trigger to interracted npc via index */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void CreateTriggerThroughIndex(int32 index);

	/** Attaches an action to interracted npc via index */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void CreateActionThroughIndex(int32 index);

	/** Passes gunshot sound as parameter for pending trigger */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void PassGunShotParameter();

	/** Passes word sound as parameter for pending trigger */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void PassWordParameter(FString word);

	/** Passes actor as parameter for pending action */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void PassActorParameter(AActor* actor);

	/** Passes location as parameter for pending action */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void PassLocationParameter(FVector location);

	/** Fetches Actors suitable to be targets of Npc's attack */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	TArray<AActor*> GetNpcAttackTargets();

	/** Fetches marked locations */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	TArray<AActor*> GetMarkedLocations();

	TArray<AActor*> GetStickers();

	/** Checks if trigger provides correct parameter for action */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	bool TriggerReturnsParameter();

	/** Passes parameter provided by trigger to action */
	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void PassTriggersParameter();

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//TSubclassOf<UUserWidget> WidgetTemplate;

	//UPROPERTY()
	//UUserWidget* WidgetInstance;

	/** Texture for suggestion stickers */
	UPROPERTY(EditAnywhere, Category = GamePlay)
	UMaterialInterface* StickerMaterial;

	/** Show gui element for conversation. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowConversationGUI(bool show);

	/** Show sound configuring gui element. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowSoundParameterGui();

	/** Show actor configuring gui element. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowActorParameterGui(const TArray<AActor*>& actors);

	/** Show text configuring gui element. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowTextParameterGui();

	/** Show sound configuring gui element. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowLocationParameterGui();

	/** Show action options gui element. */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowActionsGui();

	/** Show notifier for sound */
	UFUNCTION(BlueprintImplementableEvent, Category = "GUI")
	void ShowSoundNotifier(USound* sound);

	UFUNCTION(BlueprintCallable, Category = "HypnoToad")
	void SayWord(FString word);

	bool IsHypnotizing();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	void SetGUIMode(bool isGUI, AAICharacter* ai = NULL);
	AAICharacter* InterractsWithNPC(float range);
	AAICharacter* m_conversationWith;
	AAICharacter* m_hypnotized;
	UClass* PathPointMarkerClass;
};

