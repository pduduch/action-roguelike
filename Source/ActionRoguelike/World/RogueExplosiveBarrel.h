#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class UNiagaraSystem;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueExplosiveBarrel : public AActor
{
	GENERATED_BODY()


protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Explosive Barrel")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosive Barrel")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Explosive Barrel")
	TObjectPtr<UNiagaraSystem> TriggeredEffect;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Explosive Barrel")
	bool HasBeenTriggered = false;
	
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	void ExplosionTriggeredTimerElapsed();
	
public:
	
	virtual void PostInitializeComponents() override;
	
	ARogueExplosiveBarrel();	
	
};
