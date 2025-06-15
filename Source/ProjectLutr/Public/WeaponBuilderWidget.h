#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponPartData.h"
#include "WeaponBuilderWidget.generated.h"

class UWrapBox;
class UTextBlock;
class UWeaponSlotWidget;
class AWeaponPreviewActor;

UCLASS()
class PROJECTLUTR_API UWeaponBuilderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitializeFromBase(UWeaponPartData* InBasePart);

protected:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* SlotWrapBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatDamage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatAccuracy;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatRecoil;

	// Add more as needed...

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWeaponSlotWidget> SlotWidgetClass;

	UPROPERTY()
	UWeaponPartData* BasePart;

	UPROPERTY()
	TMap<EWeaponPartType, UWeaponPartData*> EquippedParts;

	UPROPERTY()
	AWeaponPreviewActor* PreviewActor;

	void GeneratePartSlots();
	void UpdateStatsPanel();
	void SpawnOrUpdatePreview();

	UFUNCTION()
	void HandleSlotClicked(EWeaponPartType SlotType);
};
