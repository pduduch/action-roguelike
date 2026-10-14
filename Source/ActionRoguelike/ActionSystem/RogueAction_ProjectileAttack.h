// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "Templates/SubclassOf.h"
#include "RogueAction_ProjectileAttack.generated.h"

class ARogueProjectile;
class UAnimMontage;
class USoundBase;
class UNiagaraSystem;
/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API URogueAction_ProjectileAttack : public URogueAction
{
	GENERATED_BODY()
	
	void AttackTimerElapsed();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TSubclassOf<ARogueProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(VisibleAnywhere, Category="ProjectileAttack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	
public:
	URogueAction_ProjectileAttack();
	
	virtual void StartAction() override;
};
