// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "HypnoToad.h"
#include "HypnoToadCharacter.h"
#include "Engine.h"
#include "AICharacter.h"

//////////////////////////////////////////////////////////////////////////
// AHypnoToadCharacter

AHypnoToadCharacter::AHypnoToadCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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

	//WidgetInstance = CreateWidget(this, WidgetTemplate);
	//WidgetInstance->AddToViewport();
}

void AHypnoToadCharacter::SetGUIMode(bool isGUI, AAICharacter* ai)
{
	APlayerController* plr = (APlayerController*)GetController();

	plr->bShowMouseCursor = isGUI;
	plr->bEnableClickEvents = isGUI;
	plr->bEnableMouseOverEvents = isGUI;
	this->ShowConversationGUI(isGUI);
	//FInputModeGameAndUI input;
	//input.SetLockMouseToViewport(isGUI);
	//plr->SetInputMode(input);
	//if (isGUI)
	//GEngine->GameViewport->Viewport->LockMouseToViewport(true);
}

bool HitBrush(FHitResult &Hit)
{
	return Hit.Component.Get()->IsA(UModelComponent::StaticClass());
}

// Called every frame
void AHypnoToadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* plr = (APlayerController*)GetController();
	if (plr->WasInputKeyJustPressed(EKeys::F))
	{
		if (m_conversationWith)
		{
			m_conversationWith->EndConversation();
			m_conversationWith = NULL;

			SetGUIMode(false);
		}
		else
		{
			AAICharacter* ai = InterractsWithNPC(300);
			if (ai)
			{
				m_conversationWith = ai;
				ai->ActivateConversation(this);
				SetGUIMode(true, ai);
			}
		}
	}

	if (plr->WasInputKeyJustPressed(EKeys::Q))
	{
		AAICharacter* ai = InterractsWithNPC(200);
		if (ai && !m_hypnotized)
		{
			m_hypnotized = ai;
			ai->Hypnotize(this);
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}
		else if (m_hypnotized)
		{
			m_hypnotized->StayStillWhileHypnotized();
			m_hypnotized = NULL;
			GetCharacterMovement()->MaxWalkSpeed = 600;
		}
	}
	
	if (plr->WasInputKeyJustPressed(EKeys::E))
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(plr->GetPawn());
		FHitResult Hit;
		FVector Start = plr->PlayerCameraManager->GetCameraLocation();
		FVector End = Start + (plr->PlayerCameraManager->GetCameraRotation().Vector() * 1000.0f);
		if (GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params) && Hit.ImpactNormal.Z < 0.5f && Hit.ImpactNormal.Z > -0.5f)
		{
			if (!HitBrush(Hit) && Hit.Actor.Get()->IsA(ADecalActor::StaticClass()))
			{
				GetWorld()->DestroyActor(Hit.Actor.Get());
			}
			else
			{
				FRotator rot = (-Hit.ImpactNormal).Rotation();
				rot.Roll = 90;
				ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.ImpactPoint, rot);
				decal->GetDecal()->SetDecalMaterial(StickerMaterial);
				decal->SetActorScale3D(FVector(30, 30, 30));
				decal->GetBoxComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				decal->GetBoxComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
				decal->GetBoxComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			}
		}
	}

	if (plr->WasInputKeyJustPressed(EKeys::R))
	{
		SetGUIMode(true);
		//HSound::BroadCastSound(GetWorld(), TSharedPtr<HSound>(new HWord(GetActorLocation(), TEXT("Test"))));
	}
	if (plr->WasInputKeyJustPressed(EKeys::T))
		SetGUIMode(false);

}

void AHypnoToadCharacter::SayWord(FString word)
{
	HSound::BroadCastSound(GetWorld(), TSharedPtr<HSound>(new HWord(GetActorLocation(), word)));
}

AAICharacter* AHypnoToadCharacter::InterractsWithNPC(float range)
{
	APlayerController* plr = (APlayerController*)GetController();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(plr->GetPawn());
	FHitResult Hit;
	FVector Start = plr->PlayerCameraManager->GetCameraLocation();
	FVector End = Start + (plr->PlayerCameraManager->GetCameraRotation().Vector() * (range + 400));
	
	if (!GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params))
		return NULL;
	if (HitBrush(Hit))
		return NULL;
	if (!Hit.Actor.Get()->IsA(AAICharacter::StaticClass()))
		return NULL;

	AAICharacter* ai = (AAICharacter*)Hit.Actor.Get();
	Start = GetActorLocation();
	End = Hit.ImpactPoint;
	if (!GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params) || (Hit.ImpactPoint - Start).Size() > range || Hit.Actor.Get() != ai)
		return NULL;
	return ai;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHypnoToadCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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


void AHypnoToadCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AHypnoToadCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AHypnoToadCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHypnoToadCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHypnoToadCharacter::MoveForward(float Value)
{
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

void AHypnoToadCharacter::MoveRight(float Value)
{
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

bool AHypnoToadCharacter::IsHypnotizing()
{
	return m_hypnotized != NULL;
}