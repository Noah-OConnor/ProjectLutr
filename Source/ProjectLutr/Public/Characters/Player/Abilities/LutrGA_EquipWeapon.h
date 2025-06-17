// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/LutrGameplayAbility.h"
#include "LutrGA_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ULutrGA_EquipWeapon : public ULutrGameplayAbility
{
	GENERATED_BODY()

public:
	ULutrGA_EquipWeapon();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
