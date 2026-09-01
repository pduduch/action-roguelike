#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueExplosiveBarrel.generated.h"

class UAudioComponent;
class UNiagaraComponent;
class URadialForceComponent;
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
	
	UPROPERTY(EditDefaultsOnly, Category="Explosive Barrel")
	TObjectPtr<USoundBase> TriggeredSound;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URadialForceComponent> ForceComponent;
	
	bool bHasExploded = false;
	
	FTimerHandle ExplosionTriggeredTimerHandle;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveTriggerEffect = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> ActiveTriggerSound = nullptr;
	
	void Explode();
	
public:
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	ARogueExplosiveBarrel();	
	
};
