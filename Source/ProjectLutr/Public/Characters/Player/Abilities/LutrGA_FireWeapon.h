// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/LutrGameplayAbility.h"
#include "LutrGA_FireWeapon.generated.h"

class UAbilityTask_WaitDelay;

UCLASS()
class PROJECTLUTR_API ULutrGA_FireWeapon : public ULutrGameplayAbility
{
	GENERATED_BODY()

public:
	ULutrGA_FireWeapon();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	
	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Weapon")
	TSubclassOf<UGameplayEffect> DamageEffect;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Weapon")
	float FireRate = 0.15f;

	FTimerHandle FireLoopTimerHandle;

	void PerformFire();
	void BeginFireLoop();
	void StopFireLoop();
};
