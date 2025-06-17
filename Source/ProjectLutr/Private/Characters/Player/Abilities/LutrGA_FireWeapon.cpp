// Copyright 2025 Project Lutr


#include "Characters/Player/Abilities/LutrGA_FireWeapon.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"

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

	//UE_LOG(LogTemp, Display, TEXT("ULutrGA_Fire::ActivateAbility"));

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
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
		return;

	FVector Location = Avatar->GetActorLocation();
	FRotator Rotation = Avatar->GetActorRotation();

	//UE_LOG(LogTemp, Log, TEXT("[FIRE] Actor %s fired weapon at %s"), *Avatar->GetName(), *Location.ToString());

	if (!Avatar->HasAuthority()) return;
	ACharacter* Character = Cast<ACharacter>(Avatar);
	if (!Character) return;

	UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	FRotator AimRotation;
	if (ActorInfo->PlayerController.IsValid())
	{
		AimRotation = ActorInfo->PlayerController->GetControlRotation();
	}
	else
	{
		AimRotation = Camera->GetComponentRotation(); // fallback
	}

	FVector End = Start + (AimRotation.Vector() * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Avatar);
	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;

		UE_LOG(LogTemp, Display, TEXT("[FIRE] Actor %s hit"), *HitActor->GetName());
		
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

		if (TargetASC && SourceASC && DamageEffect)
		{
			// Create effect context
			FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			// Create GE spec
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, 1.f, EffectContext);

			if (SpecHandle.IsValid())
			{
				// Optional: send dynamic damage via SetByCaller
				float DamageAmount = 20.f;
				SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DamageAmount);

				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
			}
		}
	}
}
