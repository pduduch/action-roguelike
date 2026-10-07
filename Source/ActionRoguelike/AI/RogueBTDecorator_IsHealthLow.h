// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RogueBTDecorator_IsHealthLow.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTDecorator_IsHealthLow : public UBTDecorator
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin="0.0", CampMax="1.0"))
	float LowHealthThreshold = 0.3f;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
