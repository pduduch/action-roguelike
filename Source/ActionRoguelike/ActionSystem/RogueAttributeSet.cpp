// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
	MoveSpeedMultiplier = FRogueAttribute(1.0f);
}

URoguePlayerAttributeSet::URoguePlayerAttributeSet()
{
	Rage = FRogueAttribute(0);
}

URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(450);
}

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100);
	HealthMax = FRogueAttribute(Health.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
}

void URoguePawnAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	ApplyMoveSpeed();
}

void URoguePlayerAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	Rage.Base = FMath::Max(0.0f, Rage.Base);
}

void URoguePawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue() * MoveSpeedMultiplier.GetValue();
}

URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}
