// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LutrFloatingStatusBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ULutrFloatingStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Updates the health bar percent (0-1) */
	UFUNCTION(BlueprintCallable, Category = "StatusBar")
	void SetHealthPercentage(float HealthPercent);

	/** Sets the character's name in the widget */
	UFUNCTION(BlueprintCallable, Category = "StatusBar")
	void SetCharacterName(const FText& NewName);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
};
