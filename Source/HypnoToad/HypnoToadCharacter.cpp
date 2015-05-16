// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "HypnoToad.h"
#include "HypnoToadCharacter.h"
#include "Engine.h"
#include "AICharacter.h"
#include "HTrigger.h"
#include "Word.h"
#include "GunShot.h"

//////////////////////////////////////////////////////////////////////////
// AHypnoToadCharacter

AHypnoToadCharacter::AHypnoToadCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Following code came with empty unreal 4 project
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Following block written by me

	//Find blueprint class to spawn later as location markers
	ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/MyBlueprints/PathPointMarker_Blueprint.PathPointMarker_Blueprint'"));
	if (Blueprint.Object != NULL)
		PathPointMarkerClass = (UClass*)Blueprint.Object->GeneratedClass;

	//Initialize variable values
	m_InGuiMode = false;
	InRestrictedArea = false;
	m_creatingTrigger = false;
	m_hypnoseDelay = 3;
	m_health = 5;
}

void AHypnoToadCharacter::SetGUIMode(bool isGUI)
{
	APlayerController* plr = (APlayerController*)GetController();

	plr->bShowMouseCursor = isGUI;
	plr->bEnableClickEvents = isGUI;
	plr->bEnableMouseOverEvents = isGUI;
	ShowConversationGUI(isGUI);
	m_InGuiMode = isGUI;
}

//When trace hits BSP brush, FHitResult.Actor.Get() return NULL, causing exceptions. However FHitResult.Component.Get() returns something useful.
//This helper function is used to track whether BSP brush was hit, and FHitResult.Actor.Get() shouldn't be called.
bool HitBrush(FHitResult &Hit)
{
	return Hit.Component.Get()->IsA(UModelComponent::StaticClass());
}

// Called every frame
void AHypnoToadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
		return;

	if (m_creatingTrigger)
	{
		//Progress with hypnotization
		m_hypnoseDelay -= DeltaTime;
		if (m_conversationWith && m_hypnoseDelay <= 0)
		{
			//finally done with hypnotization
			m_conversationWith->AttachPendingTrigger();
			EndHypnotization();
		}
	}

	DiscoverNPCs();
}

void AHypnoToadCharacter::ToggleConversation()
{
	if (m_conversationWith)
	{
		m_conversationWith->EndConversation();
		EndConversation();
	}
	else
	{
		StartConversation();
	}
}

void AHypnoToadCharacter::ToggleSay()
{
	if (m_conversationWith == NULL)
		SetGUIMode(!m_InGuiMode);
}

bool SurfaceIsWall(FHitResult& Hit)
{
	return Hit.ImpactNormal.Z < 0.5f && Hit.ImpactNormal.Z > -0.5f;
}

void AHypnoToadCharacter::CreateSticker(FHitResult& Hit)
{
	FRotator rot = (-Hit.ImpactNormal).Rotation();
	rot.Roll = 90;	//this is correct rotation for some reason
	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.ImpactPoint + Hit.ImpactNormal * 20, rot);
	decal->GetDecal()->SetDecalMaterial(StickerMaterial);
	decal->SetActorScale3D(FVector(30, 30, 30));
	//Box collision needed so it is visible to ai characters
	decal->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	decal->GetBoxComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	decal->GetBoxComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AHypnoToadCharacter::PlaceSticker()
{
	APlayerController* plr = (APlayerController*)GetController();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector Start = plr->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (plr->PlayerCameraManager->GetCameraRotation().Vector() * 1000.0f);
	if (GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params) && SurfaceIsWall(Hit))
	{
		//When looking at placed sticker, remove it.
		if (!HitBrush(Hit) && Hit.Actor.Get()->IsA(ADecalActor::StaticClass()))
		{
			GetWorld()->DestroyActor(Hit.Actor.Get());
		}
		else
		{
			CreateSticker(Hit);
		}
	}
}

void AHypnoToadCharacter::PlaceMarker()
{
	//sanity check
	if (!PathPointMarkerClass)
		return;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector End = GetActorLocation() - FVector::UpVector * 200;
	if (GetWorld()->LineTraceSingle(Hit, GetActorLocation(), End, ECC_Visibility, Params))
	{
		FVector* loc = new FVector(Hit.ImpactPoint);
		FRotator* rot = new FRotator(GetActorRotation());
		AActor* marker = GetWorld()->SpawnActor(PathPointMarkerClass, loc, rot);
		//PathPointMarkerClass should always be inherited from APathPoint
		if (marker->IsA(APathPoint::StaticClass()))
			((APathPoint*)marker)->PlacedByPlayer = true;
	}
}

void AHypnoToadCharacter::PutToSleep()
{
	AAICharacter* ai = InterractsWithNPC(200);
	if (ai && !m_sleepingEscort && !ai->IsAttacking())
	{
		m_sleepingEscort = ai;
		ai->Sleep(this, true);
		//Walk slower when escorting
		GetCharacterMovement()->MaxWalkSpeed = 200;
	}
	else if (m_sleepingEscort)
	{
		//When toggling, don't wake up, rather stop escorting
		m_sleepingEscort->StayStillWhileAsleep();
		m_sleepingEscort = NULL;
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
}

void AHypnoToadCharacter::DiscoverNPCs()
{
	for (TActorIterator<AAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->Discovered)
			continue;
		if (*ActorItr == InterractsWithNPC(10000))
		{
			ActorItr->Discovered = true;
		}
	}
}

void AHypnoToadCharacter::SayWord(FString word)
{
	UWord* said = NewObject<UWord>();
	said->Content = word;
	said->Origin = GetActorLocation();
	USound::BroadCastSound(GetWorld(), said);
}

AAICharacter* AHypnoToadCharacter::InterractsWithNPC(float range)
{
	APlayerController* plr = (APlayerController*)GetController();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector Start = plr->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (plr->PlayerCameraManager->GetCameraRotation().Vector() * (range + 400));
	
	//First trace from camera:
	//Following check also collects trace results in variable Hit
	if (!GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params))
		return NULL;
	if (HitBrush(Hit))
		return NULL;
	if (!Hit.Actor.Get()->IsA(AAICharacter::StaticClass()))
		return NULL;

	//Second trace from player character:
	AAICharacter* ai = (AAICharacter*)Hit.Actor.Get();
	Start = GetActorLocation();
	End = Hit.ImpactPoint;
	if (!GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params) || (Hit.ImpactPoint - Start).Size() > range || Hit.Actor.Get() != ai)
		return NULL;
	//When both traces result same ai character, return given character
	return ai;
}

void AHypnoToadCharacter::StartConversation()
{
	AAICharacter* ai = InterractsWithNPC(300);
	if (ai)
	{
		m_conversationWith = ai;
		ai->ActivateConversation(this);
		SetGUIMode(true);
	}
}

void AHypnoToadCharacter::EndConversation()
{
	m_conversationWith = NULL;
	SetGUIMode(false);
	EndHypnotization();
}

//////////////////////////////////////////////////////////////////////////
// Input

//Code came with unreal 4 project
void AHypnoToadCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("ToggleConversation", IE_Pressed, this, &AHypnoToadCharacter::ToggleConversation);
	InputComponent->BindAction("ToggleSay", IE_Pressed, this, &AHypnoToadCharacter::ToggleSay);
	InputComponent->BindAction("PlaceSticker", IE_Pressed, this, &AHypnoToadCharacter::PlaceSticker);
	InputComponent->BindAction("PlaceMarker", IE_Pressed, this, &AHypnoToadCharacter::PlaceMarker);
	InputComponent->BindAction("PutToSleep", IE_Pressed, this, &AHypnoToadCharacter::PutToSleep);

	InputComponent->BindAxis("MoveForward", this, &AHypnoToadCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHypnoToadCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AHypnoToadCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AHypnoToadCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AHypnoToadCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AHypnoToadCharacter::TouchStopped);
}

//Code came with unreal 4 project
void AHypnoToadCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (IsDead())
		return;
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

//Code came with unreal 4 project
void AHypnoToadCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

//Code came with unreal 4 project
void AHypnoToadCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//Code came with unreal 4 project
void AHypnoToadCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//Code came with unreal 4 project
void AHypnoToadCharacter::MoveForward(float Value)
{
	if (IsDead())
		return;

	if ((Controller != NULL) && (Value != 0.0f) && !m_conversationWith)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//Code came with unreal 4 project
void AHypnoToadCharacter::MoveRight(float Value)
{
	if (IsDead())
		return;

	if ((Controller != NULL) && (Value != 0.0f) && !m_conversationWith)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool AHypnoToadCharacter::IsEscorting()
{
	return m_sleepingEscort != NULL;
}

AAICharacter* AHypnoToadCharacter::HasConversationWith()
{
	return m_conversationWith;
}

bool AHypnoToadCharacter::NpcHasRoomForTriggers()
{
	if (m_conversationWith == NULL)
		return false;
	return m_conversationWith->GetActiveTriggers().Num() < m_conversationWith->MaxTriggers;
}

TArray<FString> AHypnoToadCharacter::GetNpcTriggerNames()
{
	if (m_conversationWith == NULL)
		return TArray < FString >();
	TArray<FString> arrayOfTriggerNames;
	for (HTrigger* t : m_conversationWith->GetTriggers())
		arrayOfTriggerNames.Add(t->GetMenuName());
	return arrayOfTriggerNames;
}

TArray<FString> AHypnoToadCharacter::GetNpcActiveTriggerNames()
{
	if (m_conversationWith == NULL)
		return TArray < FString >();
	TArray<FString> arrayOfTriggerNames;
	for (HTrigger* t : m_conversationWith->GetActiveTriggers())
		arrayOfTriggerNames.Add(t->GetMenuName());
	return arrayOfTriggerNames;
}

TArray<FString> AHypnoToadCharacter::GetNpcActiveActionNames()
{
	if (m_conversationWith == NULL)
		return TArray < FString >();
	TArray<FString> arrayOfActionNames;
	for (HTrigger* t : m_conversationWith->GetActiveTriggers())
	{
		if (t->GetAction() == NULL)
		{
			arrayOfActionNames.Add("None");
			continue;
		}
		arrayOfActionNames.Add(t->GetAction()->GetMenuName());
	}
	return arrayOfActionNames;
}

void AHypnoToadCharacter::CreateTriggerThroughIndex(int32 index)
{
	if (m_conversationWith == NULL)
		return;
	m_conversationWith->PrepareTriggerViaIndex(index);
}

void AHypnoToadCharacter::ChangeTriggersActionThroughIndex(int32 index)
{
	if (m_conversationWith == NULL)
		return;
	m_conversationWith->PrepareActiveTriggerViaIndex(index);
	ShowActionsGui();
}

void AHypnoToadCharacter::PassGunShotParameter()
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL)
		return;
	USound* gunShot = NewObject<UGunShot>();
	m_conversationWith->GetPendingTrigger()->SetSoundParameter(gunShot);
}

void AHypnoToadCharacter::PassWordParameter(FString word)
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL)
		return;
	UWord* said = NewObject<UWord>();
	said->Content = word;
	//if we don't have action pending, assign parameter to pending trigger
	if (m_conversationWith->GetPendingAction() == NULL)
	{
		m_conversationWith->GetPendingTrigger()->SetSoundParameter(said);
		return;
	}
	//otherwise assign to pending action
	m_conversationWith->GetPendingAction()->SetSoundParameter(said);
}

TArray<FString> AHypnoToadCharacter::GetNpcActionNames()
{
	if (m_conversationWith == NULL)
		return TArray < FString >();
	TArray<FString> arrayOfActionNames;
	for (HAction* a : m_conversationWith->GetActions())
		arrayOfActionNames.Add(a->GetMenuName());
	return arrayOfActionNames;
}

void AHypnoToadCharacter::CreateActionThroughIndex(int32 index)
{
	if (m_conversationWith == NULL)
		return;
	m_conversationWith->PrepareActionViaIndex(index);
}

TArray<AActor*> AHypnoToadCharacter::GetNpcAttackTargets()
{
	TArray<AActor*> result;
	for (TActorIterator<AAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->Discovered)
			result.Add(*ActorItr);
	}
	//player is also attack target
	result.Add(this);
	return result;
}

TArray<AActor*> AHypnoToadCharacter::GetMarkedLocations()
{
	TArray<AActor*> result;
	for (TActorIterator<APathPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		if (ActorItr->PlacedByPlayer)	result.Add(*ActorItr);
	return result;
}

TArray<AActor*> AHypnoToadCharacter::GetStickers()
{
	TArray<AActor*> result;
	for (TActorIterator<ADecalActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		result.Add(*ActorItr);
	return result;
}

void AHypnoToadCharacter::PassActorParameter(AActor* actor)
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL)
		return;
	//if we don't have action pending, assign parameter to pending trigger
	if (m_conversationWith->GetPendingAction() == NULL)
	{
		m_conversationWith->GetPendingTrigger()->SetActorParameter(actor);
		return;
	}
	//otherwise pass parameter to pending action
	m_conversationWith->GetPendingAction()->SetActorParameter(actor);
}

void AHypnoToadCharacter::PassLocationParameter(FVector location)
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL)
		return;
	TSharedPtr<FVector> vector = TSharedPtr<FVector>(new FVector(location));
	/*if (m_conversationWith->GetPendingAction() == NULL)
	{
		m_conversationWith->GetPendingTrigger()->SetVectorParameter(vector);
		return;
	}*/
	m_conversationWith->GetPendingAction()->SetVectorParameter(vector);
}

bool AHypnoToadCharacter::TriggerReturnsParameter()
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL)
		return false;
	return m_conversationWith->GetPendingTrigger()->ProvidedParameter() != NULL;
}

void AHypnoToadCharacter::PassTriggersParameter()
{
	if (m_conversationWith == NULL || m_conversationWith->GetPendingTrigger() == NULL || m_conversationWith->GetPendingAction() == NULL)
		return;
	m_conversationWith->GetPendingAction()->SetActorParameter(m_conversationWith->GetPendingTrigger()->ProvidedParameter());
}

void AHypnoToadCharacter::StartHypnotizing()
{
	m_creatingTrigger = true;
	m_hypnoseDelay = 3;
	ShowHypnotizationProgress();
}

void AHypnoToadCharacter::EndHypnotization()
{
	m_creatingTrigger = false;
	m_hypnoseDelay = 3;
}

float AHypnoToadCharacter::HypnotizationDelayStatus()
{
	return 1.0f - m_hypnoseDelay / 3.0f;
}

void AHypnoToadCharacter::Ragdollize()
{
	USkeletalMeshComponent* mesh = (USkeletalMeshComponent*)GetComponentByClass(USkeletalMeshComponent::StaticClass());
	if (mesh)
	{
		mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		mesh->SetSimulatePhysics(true);
	}
}

void AHypnoToadCharacter::LoseHealth()
{
	--m_health;
	//Check if player is dead
	if (IsDead())
	{
		Ragdollize();
		ShowGameOverScreen();
	}
}

int32 AHypnoToadCharacter::GetHealth()
{
	return m_health;
}