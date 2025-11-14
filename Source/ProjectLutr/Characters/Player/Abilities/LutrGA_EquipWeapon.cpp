// Copyright 2025 Project Lutr


#include "ProjectLutr/Characters/Player/Abilities/LutrGA_EquipWeapon.h"

#include "AbilitySystemComponent.h"
#include "ProjectLutr/Characters/Components/InventoryComponent.h"
#include "ProjectLutr/Characters/Player/LutrPlayerCharacter.h"
#include "ProjectLutr/Player/LutrPlayerState.h"
#include "ProjectLutr/Weapon/WeaponActor.h"
#include "ProjectLutr/Weapon/WeaponDataAsset.h"
#include "ProjectLutr/Weapon/WeaponInstance.h"

ULutrGA_EquipWeapon::ULutrGA_EquipWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = EAbilityInputID::EquipWeapon;
}

void ULutrGA_EquipWeapon::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// UE_LOG(LogTemp, Warning, TEXT("ULutrGA_EquipWeapon::ActivateAbility fired"));
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	ALutrPlayerCharacter* Character = Cast<ALutrPlayerCharacter>(ActorInfo->AvatarActor.Get());
	ALutrPlayerState* PlayerState = Cast<ALutrPlayerState>(ActorInfo->OwnerActor.Get());
	if (!Character || !PlayerState || !PlayerState->InventoryComponent)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("EquipAbility: Character=%s, PlayerState=%s"),
		*GetNameSafe(Character),
		*GetNameSafe(PlayerState));


	UWeaponInstance* Weapon = PlayerState->InventoryComponent->GetWeaponAt(0); 
	if (!Weapon)
	{
		// UE_LOG(LogTemp, Warning, TEXT("EquipAbility: No weapon to equip"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// UE_LOG(LogTemp, Log, TEXT("Equipping weapon: %s (Damage: %.1f)"),
	// 	*Weapon->WeaponData->GetName(),
	// 	Weapon->GetDamage());

	UWorld* World = Character->GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = nullptr;
	//SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponActor* WeaponActor = World->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass(), SpawnParams);
	if (WeaponActor)
	{
		WeaponActor->SetReplicates(true);
		WeaponActor->SetReplicatingMovement(true);
		WeaponActor->ForceNetUpdate(); // ensures timely replication
		WeaponActor->bOnlyRelevantToOwner = false;
 
		WeaponActor->SetWeaponInstance(Weapon);
		Character->EquippedWeapon = WeaponActor;

		// Attach to first person mesh "GripPoint"
		WeaponActor->AttachToComponent(
			Character->ThirdPersonMesh,
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			FName("GunSocket"));

		UE_LOG(LogTemp, Warning, TEXT("WeaponActor spawned and attached to GripPoint socket"));
	}

	if (Weapon->WeaponData && Weapon->WeaponData->FireAbilityClass)
	{
		UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayAbilitySpec Spec(Weapon->WeaponData->FireAbilityClass, 1, static_cast<int32>(EAbilityInputID::FireWeapon), Character);
			ASC->GiveAbility(Spec);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}