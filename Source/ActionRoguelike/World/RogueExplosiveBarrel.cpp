

#include "RogueExplosiveBarrel.h"

#include "NiagaraFunctionLibrary.h"
#include "ActionRoguelike/Projectiles/RogueProjectileMagic.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComponent;
}

void ARogueExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	this->OnTakePointDamage.AddDynamic(this, &ARogueExplosiveBarrel::OnDamageTaken);
}

void ARogueExplosiveBarrel::OnDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
	FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (HasBeenTriggered) return;
	
	HasBeenTriggered = true;
	
	FTimerHandle ExplosionTriggeredTimerHandle;
	const float ExplosionTriggeredDelayTime = 3.f;
	
	GetWorldTimerManager().SetTimer(ExplosionTriggeredTimerHandle, this, &ARogueExplosiveBarrel::ExplosionTriggeredTimerElapsed, ExplosionTriggeredDelayTime);
	
	// Get spawned ref, but how to destroy it later?
	// UNiagaraFunctionLibrary::SpawnSystemAttached(TriggeredEffect, MeshComponent, <socket name?>,
	//	FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
}

void ARogueExplosiveBarrel::ExplosionTriggeredTimerElapsed()
{
	// Stop sound and particles
	
	URadialForceComponent ForceComponent;
	ForceComponent.Radius = 5.f;
	ForceComponent.Falloff = ERadialImpulseFalloff::RIF_Constant;
	ForceComponent.ImpulseStrength = 20000.f;
	ForceComponent.bIgnoreOwningActor = true;
	
	ForceComponent.FireImpulse();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	Destroy();
}

