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
#include "HActionIgnore.h"
#include "HActionEndHypnotization.h"
#include "HTriggerSawPlayerInRestricted.h"
#include "HActionForgetEnemy.h"

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
	Discovered = false;
	m_health = 3;

	HTrigger* t = new HTriggerSawHypnotizedNpc(this);
	HTriggerSawHypnotizedNpc* sawHypnotizedNpc = (HTriggerSawHypnotizedNpc*)t;
	t->SetAction(new HActionDetour(this, sawHypnotizedNpc->GetHypnotizedNpcLocation()));
	HTrigger* t2 = new HTriggerSawPlayerHypnotizing(this);
	t2->SetAction(new HActionAttack(this, GetWorld()->GetFirstPlayerController()->GetCharacter()));
	HTrigger* t3 = new HTriggerHeard(this, NewObject<UGunShot>());
	HTriggerHeard* heard = (HTriggerHeard*)t3;
	t3->SetAction(new HActionDetour(this, heard->GetSoundSource()));
	HTrigger* t4 = new HTriggerSawPlayerInRestricted(this);
	t4->SetAction(new HActionAttack(this, GetWorld()->GetFirstPlayerController()->GetCharacter()));
	triggers.Add(t);
	triggers.Add(t2);
	triggers.Add(t3);
	triggers.Add(t4);

	m_availableTriggers.Add(new HTriggerSaw(this, NULL));
	m_availableTriggers.Add(new HTriggerHeard(this, NewObject<UGunShot>()));

	m_availableActions.Add(new HActionIgnore(this));
	m_availableActions.Add(new HActionEndHypnotization(this));
	m_availableActions.Add(new HActionAttack(this, NULL));
	m_availableActions.Add(new HActionDetour(this, NULL));
	m_availableActions.Add(new HActionFreeze(this));
	m_availableActions.Add(new HActionSay(this, NewObject<UWord>()));
	m_availableActions.Add(new HActionForgetEnemy(this, true));

	UNavigationSystem::SimpleMoveToActor(Controller, PPoint);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);

	if (!Armed)
	{
		for (UActorComponent* gunMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
		{
			if (!gunMesh || gunMesh->GetName() != "Gun")
				continue;
			gunMesh->DestroyComponent();
		}
	}
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (IsDead())	return;

	for (HTrigger* t : triggers)
	{
		t->Trigger();
	}

	for (USound* s : m_heardSounds)
	{
		DeleteObject(s);
	}
	m_heardSounds.Empty();

	if (m_rateOfFire > 0)
		m_rateOfFire -= DeltaTime;

	if (CanSee(m_currentEnemy) && !IsHypnotized())
		m_attacking = true;

	if (m_attacking)
	{
		if (FVector::Dist(m_currentEnemy->GetActorLocation(), m_lastEnemyPosition) > 200)
			DrawDebugBox(GetWorld(), m_lastEnemyPosition, FVector(50, 50, 100), FColor::Red);
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
				if (m_timeToCare <= 0)
					StopAttack(false);
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
		if (Discovered)
		{
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
	//const FName TraceTag("MyTraceTag");
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams Params;
	//Params.TraceTag = TraceTag;
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
		DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 0.25f, -1, 2);
		if (!Hit.Component.Get()->IsA(UModelComponent::StaticClass()))
		{
			//if (Hit.Actor->IsA(AAICharacter::StaticClass()))
			//	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Friendly fire"));
			if (Hit.Actor->IsA(AAICharacter::StaticClass()))// || Hit.Actor->IsA(AHypnoToadCharacter::StaticClass()))
			{
				AAICharacter* ai = (AAICharacter*)(Hit.Actor.Get());
				Hurt(ai);
			}
		}
	}
	USound* gunShot = NewObject<UGunShot>();
	gunShot->Origin = GetActorLocation();
	USound::BroadCastSound(GetWorld(), gunShot);
}

void AAICharacter::Hurt(AAICharacter* victim)
{
	if (!Armed)
		return;
	--victim->m_health;
	if (!victim->IsAttacking())
		victim->Attack(this);
	if (victim->IsDead())
	{
		victim->GetController()->StopMovement();
		USkeletalMeshComponent* mesh = (USkeletalMeshComponent*)victim->GetComponentByClass(USkeletalMeshComponent::StaticClass());
		if (mesh)
		{
			mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mesh->SetSimulatePhysics(true);
			StopAttack(true);
			for (UActorComponent* gunMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
			{
				if (!gunMesh || gunMesh->GetName() != "Gun")
					continue;
				gunMesh->DestroyComponent();
			}
		}
	}
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
	if (!Armed || IsHypnotized())
		return;
	m_currentEnemy = actor;
	m_timeToCare = 0;
	m_scanPosition = 0;
	EndConversation();
}

void AAICharacter::StopAttack(bool forgetEnemy)
{
	m_attacking = false;
	if (forgetEnemy)
		m_currentEnemy = NULL;
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
	if (!IsDead())
		m_heardSounds.Add(sound);
}

USound* AAICharacter::HeardSound(USound* sound)
{
	for (USound* snd : m_heardSounds)
	{
		if (snd == NULL)
			continue;
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
	m_pendingTrigger = m_availableTriggers[index]->CreateTrigger();
	m_pendingTrigger->CollectParameters();
}

void AAICharacter::PrepareActiveTriggerViaIndex(int32 index)
{
	m_pendingTrigger = triggers[index];
}

void AAICharacter::AttachPendingTrigger()
{
	if (!m_pendingTrigger || !m_pendingAction)
		return;
	m_pendingTrigger->SetAction(m_pendingAction);
	if (!triggers.Contains(m_pendingTrigger))
		triggers.Add(m_pendingTrigger);
	m_pendingTrigger = NULL;
	m_pendingAction = NULL;
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowConversationGUI(true);
}

void AAICharacter::PrepareActionViaIndex(int32 index)
{
	m_pendingAction = m_availableActions[index]->CreateAction();
	m_pendingAction->CollectParameters();
}
