// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueHealthPickup.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/RogueGameplayStatics.h"
#include "Kismet/GameplayStatics.h"


ARogueHealthPickup::ARogueHealthPickup()
{
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMeshComp"));
	PickupMeshComponent->SetCollisionProfileName("NoCollision");
	PickupMeshComponent->SetupAttachment(RootComponent);
}

void ARogueHealthPickup::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	URogueActionSystemComponent* ActionSystemComp = OtherActor->GetComponentByClass<URogueActionSystemComponent>();
	
	if (ensure(ActionSystemComp != nullptr) && !URogueGameplayStatics::IsHealthFull(ActionSystemComp))
	{
		ActionSystemComp->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, HealingAmount, Base);
		
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
		Destroy();		
	}
}
