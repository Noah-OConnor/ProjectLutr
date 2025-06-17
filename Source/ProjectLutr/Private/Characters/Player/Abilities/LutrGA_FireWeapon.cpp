// Copyright 2025 Project Lutr


#include "Characters/Player/Abilities/LutrGA_FireWeapon.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ULutrGA_FireWeapon::ULutrGA_FireWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityInputID = EAbilityInputID::FireWeapon;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void ULutrGA_FireWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Display, TEXT("ULutrGA_Fire::ActivateAbility"));

	// Begin looped fire
	PerformFire();
	BeginFireLoop();
}

void ULutrGA_FireWeapon::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void ULutrGA_FireWeapon::BeginFireLoop()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(FireLoopTimerHandle, this, &ULutrGA_FireWeapon::PerformFire, FireRate, true);
	}
}

void ULutrGA_FireWeapon::StopFireLoop()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FireLoopTimerHandle);
	}
}

void ULutrGA_FireWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	StopFireLoop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULutrGA_FireWeapon::PerformFire()
{
	// 🔫 This is where the actual firing logic goes:
	// LineTrace, spawn projectile, apply damage, etc.

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
		return;

	FVector Location = Avatar->GetActorLocation();
	FRotator Rotation = Avatar->GetActorRotation();

	UE_LOG(LogTemp, Log, TEXT("[FIRE] Actor %s fired weapon at %s"), *Avatar->GetName(), *Location.ToString());

	// TODO: Add actual fire logic (hit scan or projectile, depending on weapon type)
}
