// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/LutrGameplayAbility.h"
#include "LutrGA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ULutrGA_Jump : public ULutrGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULutrGA_Jump();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};