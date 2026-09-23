// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileTeleport.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"


ARogueProjectileTeleport::ARogueProjectileTeleport()
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ARogueProjectileTeleport::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TeleportHandle, this, &ARogueProjectileTeleport::StartTeleport, DetonateDelay);
}

void ARogueProjectileTeleport::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(TeleportHandle);
	
	StartTeleport();
}

void ARogueProjectileTeleport::StartTeleport()
{
	ProjectileMovementComponent->StopMovementImmediately();
	PlayExplosionEffectsAndSounds();
	
	LoopedNiagaraComponent->Deactivate();
	LoopedAudioComponent->Stop();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TeleportHandle, this, &ARogueProjectileTeleport::TeleportDelayElapsed, TeleportSecondaryDelay);
}

void ARogueProjectileTeleport::TeleportDelayElapsed()
{
	APawn* ProjectileCaster = GetInstigator();
	check(ProjectileCaster);
	
	ProjectileCaster->TeleportTo(GetActorLocation(), ProjectileCaster->GetActorRotation());
	
	Destroy();
}

