// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_IsHealthLow.h"

#include "AIController.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_IsHealthLow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);
	
	URogueActionSystemComponent* ActionSystemComp = Pawn->GetComponentByClass<URogueActionSystemComponent>();
	if (ensure(ActionSystemComp))
	{
		float HealthFraction = ActionSystemComp->GetAttributeValue(SharedGameplayTags::Attribute_Health) /
			ActionSystemComp->GetAttributeValue(SharedGameplayTags::Attribute_HealthMax);
		
		return HealthFraction <= LowHealthThreshold;
	}
	
	return false;
}
