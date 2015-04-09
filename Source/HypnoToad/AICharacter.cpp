// Fill out your copyright notice in the Description page of Project Settings.

#include "HypnoToad.h"
#include "AICharacter.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationPath.h"
#include "HTriggerSawHypnotizedNpc.h"
#include "HActionWakeNpc.h"
#include "HypnoToadCharacter.h"

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

	HTrigger* t = new HTriggerSawHypnotizedNpc(this);
	t->SetAction(new HActionWakeNpc(this));
	triggers.Add(t);

	UNavigationSystem::SimpleMoveToActor(Controller, PPoint);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);

}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	for (HTrigger* t : triggers)
		t->Trigger();

	if (waitTime <= 0 && !m_havingConversation && !m_hypnotizedBy)
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
		waitTime -= DeltaTime;
		if (waitTime <= 0)
			UNavigationSystem::SimpleMoveToActor(Controller, PPoint);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, 6.28f));
	}
	else if (m_hypnotizedBy && m_followsHypnotizer)
	{
		UNavigationSystem::SimpleMoveToActor(Controller, m_hypnotizedBy);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		DesiredRotation = GetActorRotation();
	}

	APlayerController* plr = GetWorld()->GetFirstPlayerController();
	if (plr->WasInputKeyJustPressed(EKeys::LeftMouseButton))
		Shoot();

}

void AAICharacter::Shoot()
{
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams Params;
	Params.TraceTag = TraceTag;
	Params.AddIgnoredActor(this);
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FVector Dir = GetActorRotation().Vector();
	FVector Up, Right;
	Dir.FindBestAxisVectors(Up, Right);
	float angle = FMath::FRandRange(0, 2 * PI);
	float radius = FMath::FRandRange(0, 10);
	Dir = Dir.RotateAngleAxis(radius * FMath::Sin(angle), Up);
	Dir = Dir.RotateAngleAxis(radius * FMath::Cos(angle), Right);
	FVector End = Start + (Dir * 1000000);
	if (GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Visibility, Params))
	{
		if (!Hit.Component.Get()->IsA(UModelComponent::StaticClass()) && Hit.Actor->IsA(AAICharacter::StaticClass()))
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Ow"));
	}
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

void AAICharacter::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AAICharacter::StaticClass()))
		return;
	AAICharacter* ai = (AAICharacter*)OtherActor;
	ai->EndHypnotization();
}