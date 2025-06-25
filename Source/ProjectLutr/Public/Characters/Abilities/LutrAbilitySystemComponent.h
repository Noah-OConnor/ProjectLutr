// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LutrAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, ULutrAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ULutrAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(ULutrAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
