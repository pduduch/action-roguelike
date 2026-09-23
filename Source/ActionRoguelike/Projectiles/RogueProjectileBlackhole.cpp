// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlackhole.h"

#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"


ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	ProjectileMovementComponent->InitialSpeed = 500.f;	
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 750.f;
	RadialForceComponent->Falloff = ERadialImpulseFalloff::RIF_Constant;
	RadialForceComponent->bIgnoreOwningActor = true;
	RadialForceComponent->ForceStrength = -750000.f;
	RadialForceComponent->bAutoActivate = true;
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Pawn should not be affected
	
	SphereComponent->SetSphereRadius(20.f);
	SphereComponent->SetCollisionProfileName("ProjectileBlackhole");
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueProjectileBlackhole::OnActorBeginOverlap);
	
	FTimerHandle BlackholeTTLHandle;
	float BlackholeTTLSeconds = 5.f;
	GetWorldTimerManager().SetTimer(BlackholeTTLHandle, this, &ARogueProjectileBlackhole::BlackholeTTLElapsed, BlackholeTTLSeconds);
}

void ARogueProjectileBlackhole::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}

void ARogueProjectileBlackhole::BlackholeTTLElapsed()
{
	Destroy();
}


