// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupActor.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ARoguePickupActor::ARoguePickupActor()
{
	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComp"));
	RootComponent = OverlapComponent;
	OverlapComponent->SetCollisionProfileName("Pickups");
	OverlapComponent->SetSphereRadius(128.0f);
}

void ARoguePickupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoguePickupActor::OnActorOverlapped);
}

void ARoguePickupActor::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}



