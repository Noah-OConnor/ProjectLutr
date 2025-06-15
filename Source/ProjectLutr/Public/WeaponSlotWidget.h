// Copyright 2023 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponPartData.h"
#include "WeaponSlotWidget.generated.h"

class UTextBlock;
class UImage;

DECLARE_DELEGATE_OneParam(FOnSlotClicked, EWeaponPartType)

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeSlot(EWeaponPartType InSlotType, UWeaponPartData* EquippedPart);
	FOnSlotClicked OnSlotClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotTypeLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* PartIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PartNameLabel;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;

	UFUNCTION()
	void HandleClick();

	UPROPERTY()
	EWeaponPartType SlotType;

	UPROPERTY()
	UWeaponPartData* CurrentPart;
};
