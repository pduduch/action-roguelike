// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_IsHealthLow.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_IsHealthLow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);
	
	URogueActionSystemComponent* ActionSystemComp = Pawn->GetComponentByClass<URogueActionSystemComponent>();
	if (ensure(ActionSystemComp))
	{
		return ActionSystemComp->GetCurrentHealthPercent() <= LowHealthThreshold;
	}
	
	return false;
}
