// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlotWidget.generated.h"

enum class EWeaponPartType : uint8;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeSlot(const EWeaponPartType SlotType);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotTypeLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* PartIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PartNameLabel;
};
