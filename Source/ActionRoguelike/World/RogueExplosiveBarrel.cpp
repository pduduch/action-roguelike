

#include "RogueExplosiveBarrel.h"

#include "NiagaraFunctionLibrary.h"
#include "ActionRoguelike/Projectiles/RogueProjectileMagic.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


ARogueExplosiveBarrel::ARogueExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComponent;
	
	ForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	ForceComponent->SetupAttachment(MeshComponent);
	ForceComponent->Radius = 750.f;
	ForceComponent->Falloff = ERadialImpulseFalloff::RIF_Constant;
	ForceComponent->ImpulseStrength = 150000.f;
	ForceComponent->bIgnoreOwningActor = true;
	ForceComponent->bAutoActivate = false;
}

float ARogueExplosiveBarrel::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (bHasExploded || GetWorldTimerManager().TimerExists(ExplosionTriggeredTimerHandle))
	{
		return ActualDamage;
	}
	
	ActiveTriggerEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(TriggeredEffect, MeshComponent, NAME_None,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	ActiveTriggerSound = UGameplayStatics::SpawnSoundAttached(TriggeredSound, MeshComponent);
	
	const float ExplosionTriggeredDelayTime = 3.f;
	GetWorldTimerManager().SetTimer(ExplosionTriggeredTimerHandle, this, &ARogueExplosiveBarrel::Explode, ExplosionTriggeredDelayTime);
	
	return ActualDamage;
}

void ARogueExplosiveBarrel::Explode()
{
	bHasExploded = true;
	
	ActiveTriggerEffect->Deactivate();
	ActiveTriggerSound->Stop();
	
	ForceComponent->FireImpulse();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
}

