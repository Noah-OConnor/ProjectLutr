// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectLutr/ProjectLutr.h"
#include "LutrGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ULutrGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULutrGameplayAbility();

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID = EAbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityID = EAbilityInputID::None;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
