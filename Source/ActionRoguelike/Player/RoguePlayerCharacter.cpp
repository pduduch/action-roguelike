// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "Projectiles/RogueProjectileMagic.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "TimerManager.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

TAutoConsoleVariable<float> CVarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.0f,
	TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 is duration)"), ECVF_Cheat);

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
	
	MuzzleSocketName = "Muzzle_01";
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARoguePlayerCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, PrimaryProjectileClass);
	EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, SecondaryProjectileClass);
	EnhancedInput->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, SpecialProjectileClass);
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	
	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Sideways
	FVector RighDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RighDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::StartProjectileAttack(TSubclassOf<ARogueProjectile> ProjectileClass)
{
	PlayAnimMontage(AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ARoguePlayerCharacter::AttackTimerElapsed, ProjectileClass);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, TimerDelegate, AttackDelayTime, false);
}

void ARoguePlayerCharacter::AttackTimerElapsed(TSubclassOf<ARogueProjectile> ProjectileClass)
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FVector EyeLocation = CameraComponent->GetComponentLocation();
	FRotator EyeRotation = GetControlRotation();
	
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.0f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	UWorld* World = GetWorld();
	
	FVector AdjustedTargetLocation;
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		AdjustedTargetLocation = Hit.Location;
	}
	else
	{
		AdjustedTargetLocation = TraceEnd;		
	}
	
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation(); 
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	MoveIgnoreActorAdd(NewProjectile);
	
	float DebugDrawDuration = CVarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
	
#if !UE_BUILD_SHIPPING
	if (DebugDrawDuration > 0.0f)
	{
		// Hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);
	
		// Adjustment line trace
		DrawDebugLine(World,EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
	
		// New projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DebugDrawDuration);
	
		// Original path of the projectile
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (GetControlRotation().Vector() * 5000.0f), FColor::Purple,
			false, DebugDrawDuration);
	}
#endif
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontage);
	}
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyHealthChange(-ActualDamage);
	
	return ActualDamage;
}

