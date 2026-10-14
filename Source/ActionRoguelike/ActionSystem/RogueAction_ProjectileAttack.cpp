// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction_ProjectileAttack.h"

#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueActionSystemComponent.h"
#include "RogueGameTypes.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"

TAutoConsoleVariable<float> CVarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.0f,
	TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 is duration)"), ECVF_Cheat);

URogueAction_ProjectileAttack::URogueAction_ProjectileAttack()
{
	MuzzleSocketName = "Muzzle_01";
}

void URogueAction_ProjectileAttack::StartAction()
{
	Super::StartAction();
	
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	Character->PlayAnimMontage(AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	FTimerHandle AttackTimerHandle;
	const float AttackDelayTime = 0.2f;
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ThisClass::AttackTimerElapsed, AttackDelayTime, false);
}


void URogueAction_ProjectileAttack::AttackTimerElapsed()
{
	URogueActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = CastChecked<ACharacter>(ActionComp->GetOwner());
	
	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Character->GetController()->GetPlayerViewPoint(EyeLocation, EyeRotation);
	
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.0f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	
	UWorld* World = GetWorld();
	
	FVector AdjustedTargetLocation;
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		AdjustedTargetLocation = Hit.Location;
	}
	else
	{
		AdjustedTargetLocation = TraceEnd;		
	}
	
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation(); 
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	Character->MoveIgnoreActorAdd(NewProjectile);
	
	float DebugDrawDuration = CVarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
	
#if !UE_BUILD_SHIPPING
	if (DebugDrawDuration > 0.0f)
	{
		// Hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);
	
		// Adjustment line trace
		DrawDebugLine(World,EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
	
		// New projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DebugDrawDuration);
	
		// Original path of the projectile
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (EyeRotation.Vector() * 5000.0f), FColor::Purple,
			false, DebugDrawDuration);
	}
#endif
}
