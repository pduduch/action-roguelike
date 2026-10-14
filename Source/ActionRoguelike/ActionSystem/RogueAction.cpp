// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "RogueActionSystemComponent.h"
#include "Logging/StructuredLog.h"

void URogueAction::StartAction()
{
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName}",
		("ActionName", ActionName));
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}
