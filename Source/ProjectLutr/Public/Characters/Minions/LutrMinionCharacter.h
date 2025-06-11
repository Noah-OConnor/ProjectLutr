// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LutrCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "LutrMinionCharacter.generated.h"

/**
 * An AI controlled minion character.
 */
UCLASS()
class PROJECTLUTR_API ALutrMinionCharacter : public ALutrCharacterBase
{
	GENERATED_BODY()

public:
	ALutrMinionCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	class ULutrAbilitySystemComponent* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY()
	class ULutrAttributeSetBase* HardRefAttributeSetBase;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|UI")
	TSubclassOf<class ULutrFloatingStatusBarWidget> UIFloatingStatusBarClass;

	UPROPERTY()
	class ULutrFloatingStatusBarWidget* UIFloatingStatusBar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProjectLutr|UI")
	class UWidgetComponent* UIFloatingStatusBarComponent;

	FDelegateHandle HealthChangedDelegateHandle;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
