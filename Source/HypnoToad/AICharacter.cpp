// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "HTriggerSawHypnotizedNpc.h"
#include "HActionDetour.h"
#include "HTriggerSawPlayerHypnotizing.h"
#include "HActionAttack.h"
#include "HTriggerHeard.h"
#include "HActionFreeze.h"
#include "HTriggerSaw.h"
#include "HypnoToadCharacter.h"
#include "GunShot.h"
#include "Word.h"
#include "HActionSay.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AAICharacter::~AAICharacter()
{
	for (HTrigger* t : triggers)
		delete t;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	PPoint = StartPPoint;
	DesiredRotation = GetActorRotation();
	m_havingConversation = false;
	m_hypnotizedBy = NULL;
	m_attacking = false;

	HTrigger* t = new HTriggerSawHypnotizedNpc(this);
	HTriggerSawHypnotizedNpc* sawHypnotizedNpc = (HTriggerSawHypnotizedNpc*)t;
	t->SetAction(new HActionDetour(this, sawHypnotizedNpc->GetHypnotizedNpcLocation()));
	HTrigger* t2 = new HTriggerSawPlayerHypnotizing(this);
	t2->SetAction(new HActionAttack(this, GetWorld()->GetFirstPlayerController()->GetCharacter()));
	HTrigger* t3 = new HTriggerHeard(this, NewObject<UGunShot>());
	HTriggerHeard* heard = (HTriggerHeard*)t3;
	t3->SetAction(new HActionDetour(this, heard->GetSoundSource()));
	triggers.Add(t);
	triggers.Add(t2);
	triggers.Add(t3);

	m_availableTriggers.Add(new HTriggerSaw(this, NULL));
	m_availableTriggers.Add(new HTriggerHeard(this, NewObject<UGunShot>()));

	m_availableActions.Add(new HActionAttack(this, NULL));
	m_availableActions.Add(new HActionDetour(this, NULL));
	m_availableActions.Add(new HActionFreeze(this));
	m_availableActions.Add(new HActionSay(this, NewObject<UWord>()));

	UNavigationSystem::SimpleMoveToActor(Controller, PPoint);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	for (HTrigger* t : triggers)
	{
		t->Trigger();
	}

	m_heardSounds.Empty();

	if (m_rateOfFire > 0)
		m_rateOfFire -= DeltaTime;

	if (CanSee(m_currentEnemy))
		m_attacking = true;

	if (m_attacking)
	{
		DrawDebugBox(GetWorld(), m_lastEnemyPosition, FVector(100, 100, 100), FColor::Red);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
		m_rebuildPathTime -= DeltaTime;
		if ((GetActorLocation() - m_currentEnemy->GetActorLocation()).Size() < 700 && CanSee(m_currentEnemy))
			GetController()->StopMovement();
		else
		{
			if (m_rebuildPathTime <= 0)
				UNavigationSystem::SimpleMoveToLocation(Controller, m_lastEnemyPosition);
		}
		if (CanSee(m_currentEnemy))
		{
			if (!((ACharacter*)m_currentEnemy)->GetMovementComponent()->IsFalling())
				m_lastEnemyPosition = m_currentEnemy->GetActorLocation();
			Shoot();
			//GetCharacterMovement()->bOrientRotationToMovement = false;
			//SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
			m_timeToCare = 5;
		}
		else
		{
			//GetCharacterMovement()->bOrientRotationToMovement = true;
			if (GetVelocity().Size() > 400)
				DesiredRotation = GetVelocity().Rotation();
			else
				DesiredRotation.Yaw -= 60 * FMath::Sin(m_scanPosition - ((2 * PI * DeltaTime) / 5));
			DesiredRotation.Pitch = 0;
			DesiredRotation.Yaw += 60 * FMath::Sin(m_scanPosition);
			m_scanPosition += (2 * PI * DeltaTime) / 5;
			if (m_scanPosition >= 2 * PI)
				m_scanPosition -= 2 * PI;
			if (UNavigationSystem::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), m_lastEnemyPosition)->GetPathLength() < 300)
			{
				m_timeToCare -= DeltaTime;
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::SanitizeFloat(m_timeToCare));
				if (m_timeToCare <= 0)
					m_attacking = false;
			}
		}
		if (m_rebuildPathTime <= 0)
			m_rebuildPathTime = 0.5f;
	}
	else if (waitTime <= 0 && !m_havingConversation && !m_hypnotizedBy)
	{
		m_rebuildPathTime -= DeltaTime;
		if (m_rebuildPathTime <= 0)
		{
			UNavigationSystem::SimpleMoveToActor(Controller, PPoint);
			m_rebuildPathTime = 0.5f;
		}
		UNavigationPath* path = UNavigationSystem::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PPoint);
		if (path && path->PathPoints.Num() > 0)
		{
			FVector prev = path->PathPoints[0];
			for (FVector v : path->PathPoints)
			{
				DrawDebugLine(GetWorld(), prev, v, FColor::Green, false, -1.f, -1, 3);
				prev = v;
			}
		}
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
	}
	else if (!m_hypnotizedBy)
	{
		if (!m_havingConversation)
		{
			waitTime -= DeltaTime;
			if (waitTime <= 0)
				UNavigationSystem::SimpleMoveToActor(Controller, PPoint);
		}
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
	}
	else if (m_hypnotizedBy && m_followsHypnotizer)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, m_hypnotizedBy);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
	}

	GetCharacterMovement()->MaxWalkSpeed = m_attacking ? 500 : 200;
}

void AAICharacter::Shoot()
{
	m_scanPosition = 0;
	FVector Dir;
	if (m_currentEnemy)
	{
		Dir = (m_currentEnemy->GetActorLocation() - GetActorLocation());
		Dir.Normalize();
		FVector Dir2d = Dir;
		Dir2d.Z = 0;
		Dir2d.Normalize();
		DesiredRotation = Dir2d.Rotation();
	}
	else
	{
		Dir = GetActorRotation().Vector();
	}
	if (m_rateOfFire > 0)
		return;
	m_rateOfFire = 0.5f;
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams Params;
	Params.TraceTag = TraceTag;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector Up, Right;
	Dir.FindBestAxisVectors(Up, Right);
	float angle = FMath::FRandRange(0, 2 * PI);
	float radius = FMath::FRandRange(0, 10);
	Dir = Dir.RotateAngleAxis(radius * FMath::Sin(angle), Up);
	Dir = Dir.RotateAngleAxis(radius * FMath::Cos(angle), Right);
	FVector End = Start + (Dir * 1000000);
	if (GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params))
	{
		if (!Hit.Component.Get()->IsA(UModelComponent::StaticClass()))
		{
			if (Hit.Actor->IsA(AAICharacter::StaticClass()))
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Friendly fire"));
			if (Hit.Actor->IsA(AHypnoToadCharacter::StaticClass()))
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Ow"));
		}
	}
	USound* gunShot = NewObject<UGunShot>();
	gunShot->Origin = GetActorLocation();
	USound::BroadCastSound(GetWorld(), gunShot);
}

bool AAICharacter::CanSee(AActor* actor)
{
	if (!actor)	return false;
	FVector diff = actor->GetActorLocation() - GetActorLocation();
	diff.Normalize();
	float dot = FVector::DotProduct(GetActorRotation().Vector(), diff);
	if (dot < 0.7f)
		return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;

	return GetWorld()->LineTraceSingle(Hit, GetActorLocation(), actor->GetActorLocation(), ECC_Visibility, Params)
		&& Hit.Actor.Get() == actor;
}

void AAICharacter::Attack(AActor* actor)
{
	m_attacking = true;
	m_currentEnemy = actor;
	m_lastEnemyPosition = m_currentEnemy->GetActorLocation();
	m_timeToCare = 5;
	m_scanPosition = 0;
}

bool AAICharacter::IsAttacking()
{
	return m_attacking;
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

APathPoint* AAICharacter::GetNextPPoint()
{
	return PPoint;
}

void AAICharacter::SetNextPPoint(APathPoint* pp)
{
	PPoint = pp;
	m_rebuildPathTime = 0;
}

void AAICharacter::WaitAndHeadToNextPoint(APathPoint* PrevPoint)
{
	waitTime = PrevPoint->waitTime;
	m_rebuildPathTime = 0;
	PPoint = (APathPoint*)PrevPoint->NextPPoint;
	DesiredRotation = PrevPoint->GetActorRotation();
}

void AAICharacter::ActivateConversation(AHypnoToadCharacter* plr)
{
	GetController()->StopMovement();
	m_havingConversation = true;
	
	FVector vec = plr->GetActorLocation() - GetActorLocation();
	vec.Normalize();
	DesiredRotation = vec.Rotation();
	vec = DesiredRotation.Euler();
	vec.X = 0;
	vec.Y = 0;
	DesiredRotation = FRotator::MakeFromEuler(vec);
}

void AAICharacter::EndConversation()
{
	m_havingConversation = false;
	m_pendingAction = NULL;
	m_pendingTrigger = NULL;
}

void AAICharacter::Hypnotize(AHypnoToadCharacter* plr)
{
	m_hypnotizedBy = plr;
	m_followsHypnotizer = true;
}

void AAICharacter::EndHypnotization()
{
	m_hypnotizedBy = NULL;
	m_followsHypnotizer = false;
}

void AAICharacter::StayStillWhileHypnotized()
{
	m_followsHypnotizer = false;
	GetController()->StopMovement();
}

bool AAICharacter::IsHypnotized()
{
	return m_hypnotizedBy != NULL;
}

void AAICharacter::HearSound(USound* sound)
{
	m_heardSounds.Add(sound);
}

USound* AAICharacter::HeardSound(USound* sound)
{
	for (USound* snd : m_heardSounds)
	{
		if (*snd == sound)
			return snd;
	}
	return NULL;
}

void AAICharacter::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AAICharacter::StaticClass()))
		return;
	AAICharacter* ai = (AAICharacter*)OtherActor;
	ai->EndHypnotization();
}

void AAICharacter::PrepareTriggerViaIndex(int32 index)
{
	m_pendingTrigger = m_availableTriggers[index];
	m_pendingTrigger->CollectParameters();
}

void AAICharacter::AttachPendingTrigger()
{
	if (!m_pendingTrigger || !m_pendingAction)
		return;
	HTrigger* t = m_pendingTrigger->CreateTrigger();
	t->SetAction(m_pendingAction->CreateAction());
	triggers.Add(t);
	m_pendingTrigger = NULL;
	m_pendingAction = NULL;
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowConversationGUI(true);
}

void AAICharacter::PrepareActionViaIndex(int32 index)
{
	m_pendingAction = m_availableActions[index];
	m_pendingAction->CollectParameters();
}
