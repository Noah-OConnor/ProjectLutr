// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "FPSPlayerState.generated.h"

//class UFPSAbilitySystemComponent;
//class UPlayerAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API AFPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFPSPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UPlayerAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool IsAlive() const;

	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

protected:
	UPROPERTY()
	class UFPSAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UPlayerAttributeSet* AttributeSet;
};
