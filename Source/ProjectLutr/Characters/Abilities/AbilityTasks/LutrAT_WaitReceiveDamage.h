// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LutrAT_WaitReceiveDamage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWaitReceiveDamageDelegate, class ULutrAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * Waits until the Ability Owner receives damage.
 */
UCLASS()
class PROJECTLUTR_API ULutrAT_WaitReceiveDamage : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FWaitReceiveDamageDelegate OnDamage;

	virtual void Activate() override;

	UFUNCTION()
	void OnDamageReceived(class ULutrAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	// Wait until the ability owner receives damage.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULutrAT_WaitReceiveDamage* WaitReceiveDamage(UGameplayAbility* OwningAbility, bool TriggerOnce);

protected:
	bool TriggerOnce;

	virtual void OnDestroy(bool AbilityIsEnding) override;
};
