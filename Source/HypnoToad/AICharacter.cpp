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
#include "HTriggerNear.h"
#include "HActionShoot.h"
#include "HActionSleep.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Initialize variables
	m_ppoint = StartPPoint;
	DesiredRotation = GetActorRotation();
	m_havingConversation = false;
	m_hypnotizedBy = NULL;
	m_attacking = false;
	Discovered = false;
	m_health = 3;
	MaxTriggers = 8;

	SetupDefaultTriggersActions();

	UNavigationSystem::SimpleMoveToActor(Controller, m_ppoint);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);

	if (!Armed)
		HideWeapon();
}

void AAICharacter::SetupDefaultTriggersActions()
{
	//Default trigger-action pairs
	HTriggerSawHypnotizedNpc* t = new HTriggerSawHypnotizedNpc(this);
	t->SetIndefinateAction(new HActionDetour(this, t->GetHypnotizedNpcLocation()));
	HTrigger* t2 = new HTriggerSawPlayerHypnotizing(this);
	t2->SetIndefinateAction(new HActionAttack(this, GetWorld()->GetFirstPlayerController()->GetCharacter()));
	HTriggerHeard* t3 = new HTriggerHeard(this, NewObject<UGunShot>());
	t3->SetIndefinateAction(new HActionDetour(this, t3->GetSoundSource()));
	HTrigger* t4 = new HTriggerSawPlayerInRestricted(this);
	t4->SetIndefinateAction(new HActionAttack(this, GetWorld()->GetFirstPlayerController()->GetCharacter()));
	m_triggers.Add(t);
	m_triggers.Add(t2);
	m_triggers.Add(t3);
	m_triggers.Add(t4);

	//Trigger templates to use
	m_availableTriggers.Add(new HTriggerSaw(this, NULL));
	m_availableTriggers.Add(new HTriggerHeard(this, NULL));
	m_availableTriggers.Add(new HTriggerNear(this, NULL));

	//Action templates to use through conversation
	m_availableLightActions.Add(new HActionDetour(this, NULL, 1000));
	m_availableLightActions.Add(new HActionFreeze(this));
	m_availableLightActions.Add(new HActionSay(this, NULL));
	m_availableLightActions.Add(new HActionShoot(this));

	//Action templates to use through sleep
	m_availableHeavyActions.Add(new HActionEndHypnotization(this));
	m_availableHeavyActions.Add(new HActionIgnore(this));
	m_availableHeavyActions.Add(new HActionSleep(this, NULL));
	m_availableHeavyActions.Add(new HActionAttack(this, NULL));
	m_availableHeavyActions.Add(new HActionForgetEnemy(this, 5, true));
	m_availableHeavyActions.Add(new HActionDetour(this, NULL, 7000));
	m_availableHeavyActions.Add(new HActionFreeze(this, 5));
	m_availableHeavyActions.Add(new HActionSay(this, NULL));
	m_availableHeavyActions.Add(new HActionShoot(this));
}

void AAICharacter::Attack()
{
	m_attacking = true;
	//End any conversations when deciding to attack
	EndConversation();
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (plr->HasConversationWith() == this)
		plr->EndConversation();
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (IsDead())	return;

	RunTriggers();
	ClearRemovedTriggers();

	ClearHeardSounds();

	if (m_rateOfFire > 0)
		m_rateOfFire -= DeltaTime;

	//When saw enemy while lucid, go to attack state
	if (CanSee(m_currentEnemy) && !IsAsleep())
	{
		Attack();
	}

	if (m_attacking)
	{
		AttackTick(DeltaTime);
	}
	else if (m_waitTime <= 0 && !m_havingConversation && !m_hypnotizedBy)
	{
		PatrolTick(DeltaTime);
	}
	else if (!m_hypnotizedBy)
	{
		if (!m_havingConversation)
		{
			WaitTick(DeltaTime);
		}
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
	}
	else if (m_hypnotizedBy && m_followsHypnotizer)
	{
		FollowTick(DeltaTime);
	}

	//Run when attacking, walk otherwise
	GetCharacterMovement()->MaxWalkSpeed = m_attacking ? 500 : 200;
}

void AAICharacter::RunTriggers()
{
	for (HTrigger* t : m_triggers)
	{
		t->Trigger();
	}
}

void AAICharacter::ClearRemovedTriggers()
{
	for (HTrigger* t : m_triggersToRemove)
	{
		m_triggers.Remove(t);
	}
	m_triggersToRemove.Empty();
}

void AAICharacter::ClearHeardSounds()
{
	for (USound* s : m_heardSounds)
	{
		DeleteObject(s);
	}
	m_heardSounds.Empty();
}

void AAICharacter::AttackTick(float DeltaTime)
{
	//Draw box when target's position is far enough away from last seen position
	if (FVector::Dist(m_currentEnemy->GetActorLocation(), m_lastEnemyPosition) > 200)
		DrawDebugBox(GetWorld(), m_lastEnemyPosition, FVector(50, 50, 100), FColor::Red);

	//Npc is aiming at all directions, or looking around when lost enemy.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//Interpolate rotation
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));

	m_rebuildPathTime -= DeltaTime;
	
	//Stop and shoot when target is seen and close enough
	if (FVector::Dist(GetActorLocation(), m_currentEnemy->GetActorLocation()) < 700 && CanSee(m_currentEnemy))
		GetController()->StopMovement();
	else
	{
		//Run to last seen enemy position otherwise
		if (m_rebuildPathTime <= 0)
			UNavigationSystem::SimpleMoveToLocation(Controller, m_lastEnemyPosition);
	}

	if (CanSee(m_currentEnemy))
	{
		//Don't try to navigate to the position in the air.
		if (m_currentEnemy->IsA(ACharacter::StaticClass()) && !((ACharacter*)m_currentEnemy)->GetMovementComponent()->IsFalling())
			m_lastEnemyPosition = m_currentEnemy->GetActorLocation();

		Shoot();
		//reset care timer
		m_timeToCare = 5;
	}
	else
	{
		//Set base direction as direction of the movement velocity
		if (GetVelocity().Size() > 400)
		{
			DesiredRotation = GetVelocity().Rotation();
		}
		else
		{
			//Otherwise we might get zero vector, so direction can't be derived.
			//In that case revert to direction in the last tick
			DesiredRotation.Yaw -= 60 * FMath::Sin(m_scanPosition - ((2 * PI * DeltaTime) / 5));
		}
		//Npc doesn't look up and down
		DesiredRotation.Pitch = 0;
		
		//Look left and right in a sine wave
		DesiredRotation.Yaw += 60 * FMath::Sin(m_scanPosition);
		m_scanPosition += (2 * PI * DeltaTime) / 5;
		//Loop around on 2*PI radians
		if (m_scanPosition >= 2 * PI)
			m_scanPosition -= 2 * PI;
		
		//When close to last seen enemy position, wait a bit
		if (UNavigationSystem::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), m_lastEnemyPosition)->GetPathLength() < 300)
		{
			m_timeToCare -= DeltaTime;
			//If waited enough, leave attack state, but remember the enemy
			if (m_timeToCare <= 0)
				StopAttack(false);
		}
	}
	if (m_rebuildPathTime <= 0)
		m_rebuildPathTime = 0.5f;
}

void AAICharacter::PatrolTick(float DeltaTime)
{
	m_rebuildPathTime -= DeltaTime;
	if (m_rebuildPathTime <= 0)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, m_ppoint);
		m_rebuildPathTime = 0.5f;
	}

	VisualizePath();

	//look where you're moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	DesiredRotation = GetActorRotation();
}

void AAICharacter::VisualizePath()
{
	if (!Discovered)
		return;
	
	//Couldn't find how to get the path character currently is following, so i'm recalculating it instead.
	UNavigationPath* path = UNavigationSystem::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), m_ppoint);
	if (path && path->PathPoints.Num() > 0)
	{
		FVector prev = path->PathPoints[0];
		//Connecting the dots
		for (FVector v : path->PathPoints)
		{
			DrawDebugLine(GetWorld(), prev, v, FColor::Green, false, -1.f, -1, 3);
			prev = v;
		}
	}
}

void AAICharacter::WaitTick(float DeltaTime)
{
	m_waitTime -= DeltaTime;
	//When waited enough, return to patrol state
	if (m_waitTime <= 0)
		UNavigationSystem::SimpleMoveToActor(Controller, m_ppoint);
}

void AAICharacter::FollowTick(float DeltaTime)
{
	float pathDist = UNavigationSystem::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), m_hypnotizedBy)->GetPathLength();
	//Only follow hypnotizer when close to him (when escorted)
	if (pathDist < 1000)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, m_hypnotizedBy);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
	}
	else
		StayStillWhileAsleep();
}

FVector AAICharacter::FaceEnemy()
{
	FVector Dir;
	//When has target, face it and shoot at its direction
	if (m_currentEnemy && m_attacking)
	{
		Dir = (m_currentEnemy->GetActorLocation() - GetActorLocation());
		Dir.Normalize();
		FVector Dir2d = Dir;
		Dir2d.Z = 0;
		Dir2d.Normalize();
		//Direction is changed here
		DesiredRotation = Dir2d.Rotation();
	}
	else
	{
		//Otherwise shoot wherever currently looking
		Dir = GetActorRotation().Vector();
	}
	return Dir;
}

FVector AAICharacter::ApplySpread(FVector Dir)
{
	FVector Up, Right;
	Dir.FindBestAxisVectors(Up, Right);
	float angle = FMath::FRandRange(0, 2 * PI);
	float radius = FMath::FRandRange(0, 10);
	Dir = Dir.RotateAngleAxis(radius * FMath::Sin(angle), Up);
	Dir = Dir.RotateAngleAxis(radius * FMath::Cos(angle), Right);
	return Dir;
}

void AAICharacter::Shoot()
{
	//Can't shoot if not armed
	if (!Armed)
		return;
	//Don't scan when shooting
	m_scanPosition = 0;
	//Direction is changed even if it's not the time to shoot yet due to rate of fire
	FVector Dir = FaceEnemy();

	if (m_rateOfFire > 0)
		return;
	//Reset rate of fire
	m_rateOfFire = 0.5f;
		
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector Start = GetActorLocation();
	Dir = ApplySpread(Dir);
	FVector End = Start + (Dir * 1000000);

	if (GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params))
	{
		//Draw bullet trajectory, although it looks like a laser
		DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Red, false, 0.25f, -1, 2);
		//Prevents crash in case it hits BSP brush, check HypnoToadcharacter.cpp for details
		if (!Hit.Component.Get()->IsA(UModelComponent::StaticClass()))
		{
			if (Hit.Actor->IsA(AAICharacter::StaticClass()))
			{
				AAICharacter* ai = (AAICharacter*)(Hit.Actor.Get());
				Hurt(ai);
			}
			if (Hit.Actor->IsA(AHypnoToadCharacter::StaticClass()))
			{
				AHypnoToadCharacter* plr = (AHypnoToadCharacter*)(Hit.Actor.Get());
				plr->LoseHealth();
				if (plr->IsDead())
					StopAttack(true);
			}
		}
	}

	//Broadcast gunshot sound
	USound* gunShot = NewObject<UGunShot>();
	gunShot->Origin = GetActorLocation();
	USound::BroadCastSound(GetWorld(), gunShot);
}

void AAICharacter::Hurt(AAICharacter* victim)
{
	if (!Armed)
		return;

	--victim->m_health;
	//Infighting
	if (!victim->IsAttacking())
		victim->SetEnemy(this);
	//Check if victim is dead
	if (victim->IsDead())
	{
		victim->GetController()->StopMovement();
		victim->Ragdollize();
		StopAttack(true);
		victim->HideWeapon();
	}
}

void AAICharacter::Ragdollize()
{
	USkeletalMeshComponent* mesh = (USkeletalMeshComponent*)GetComponentByClass(USkeletalMeshComponent::StaticClass());
	if (mesh)
	{
		mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		mesh->SetSimulatePhysics(true);
	}
}

bool AAICharacter::CanSee(AActor* actor)
{
	if (!actor)	return false;
	
	FVector diff = actor->GetActorLocation() - GetActorLocation();
	diff.Normalize();
	//Actor in viewcone
	float dot = FVector::DotProduct(GetActorRotation().Vector(), diff);
	if (dot < 0.7f)
		return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult Hit;

	return GetWorld()->LineTraceSingle(Hit, GetActorLocation(), actor->GetActorLocation(), ECC_Visibility, Params)
		&& Hit.Actor.Get() == actor;
}

void AAICharacter::SetEnemy(AActor* actor)
{
	if (!Armed || IsAsleep())
		return;
	if (actor->IsA(AAICharacter::StaticClass()) && ((AAICharacter*)actor)->IsDead())
		return;
	if (actor->IsA(AHypnoToadCharacter::StaticClass()) && ((AHypnoToadCharacter*)actor)->IsDead())
		return;
	m_currentEnemy = actor;
	m_timeToCare = 0;
	m_scanPosition = 0;
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
	return m_ppoint;
}

void AAICharacter::SetNextPPoint(APathPoint* pp)
{
	m_ppoint = pp;
	m_rebuildPathTime = 0;
}

void AAICharacter::WaitAndHeadToNextPoint(APathPoint* PrevPoint)
{
	m_waitTime = PrevPoint->waitTime;
	m_rebuildPathTime = 0;
	m_ppoint = (APathPoint*)PrevPoint->NextPPoint;
	DesiredRotation = PrevPoint->GetActorRotation();
}

void AAICharacter::ActivateConversation(AHypnoToadCharacter* plr)
{
	GetController()->StopMovement();
	m_havingConversation = true;
	
	//face player
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

void AAICharacter::Sleep(AHypnoToadCharacter* plr, bool followPlayer)
{
	m_hypnotizedBy = plr;
	if (followPlayer)
		m_followsHypnotizer = true;
	if (plr->HasConversationWith() == this)
		plr->EndConversation();
}

void AAICharacter::EndSleep()
{
	m_hypnotizedBy = NULL;
	m_followsHypnotizer = false;
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter();
	if (plr->HasConversationWith() == this)
		plr->EndConversation();
}

void AAICharacter::StayStillWhileAsleep()
{
	m_followsHypnotizer = false;
	GetController()->StopMovement();
}

bool AAICharacter::IsAsleep()
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

void AAICharacter::RemoveActiveTrigger(HTrigger* trigger)
{
	m_triggersToRemove.Add(trigger);
}

void AAICharacter::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AAICharacter::StaticClass()))
		return;
	//When touched sleeping ai character, wake it up
	AAICharacter* ai = (AAICharacter*)OtherActor;
	ai->EndSleep();
}

void AAICharacter::PrepareTriggerViaIndex(int32 index)
{
	m_pendingTrigger = m_availableTriggers[index]->CreateTrigger();
	m_pendingTrigger->CollectParameters();
}

void AAICharacter::PrepareActiveTriggerViaIndex(int32 index)
{
	m_pendingTrigger = m_triggers[index];
}

void AAICharacter::RemoveActiveTriggerViaIndex(int32 index)
{
	if (m_triggers[index]->GetDefaultAction() != NULL)
	{
		m_triggers[index]->DiscardTrigger();	//This will revert to default action
		return;
	}
	m_triggers.RemoveAt(index);
}

void AAICharacter::AttachPendingTrigger()
{
	if (!m_pendingTrigger || !m_pendingAction)
		return;
	m_pendingTrigger->SetAction(m_pendingAction);
	//Check if pending trigger is new
	if (!m_triggers.Contains(m_pendingTrigger))
		m_triggers.Add(m_pendingTrigger);
	m_pendingTrigger = NULL;
	m_pendingAction = NULL;
	AHypnoToadCharacter* plr = (AHypnoToadCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter();
	plr->ShowConversationGUI(true);
}

void AAICharacter::PrepareActionViaIndex(int32 index)
{
	m_pendingAction = GetActions()[index]->CreateAction();
	m_pendingAction->CollectParameters();
}

TArray<HAction*> AAICharacter::GetActions()
{
	if (IsAsleep())
		return m_availableHeavyActions;
	return m_availableLightActions;
}

bool AAICharacter::IsDead()
{
	return m_health <= 0;
}

bool AAICharacter::HasEnemy()
{
	return m_currentEnemy != NULL;
}

AActor* AAICharacter::GetEnemy()
{
	return m_currentEnemy;
}

bool AAICharacter::IsInfluenced()
{
	if (m_triggers.Num() > 4)
		return true;
	for (HTrigger* t : m_triggers)
		if (t->GetAction() != t->GetDefaultAction())	return true;
	return false;
}

bool AAICharacter::IsMaxedInfluenced()
{
	return m_triggers.Num() >= MaxTriggers;
}

void AAICharacter::HideWeapon()
{
	for (UActorComponent* gunMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
	{
		//Hacky way to find component by name
		if (!gunMesh || gunMesh->GetName() != "Gun")
			continue;
		gunMesh->DestroyComponent();
	}
}

bool AAICharacter::IsWaiting()
{
	return m_waitTime > 0;
}

void AAICharacter::Wait(float Time)
{
	m_waitTime = Time;
	GetController()->StopMovement();
}