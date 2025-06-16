// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWeaponBuilderWidget;
class UUniformGridPanel;
class UWeaponPartData;
class UWeaponPartCardWidget;

UCLASS()
class PROJECTLUTR_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Call this from controller to fill the UI
	UFUNCTION(BlueprintCallable)
	void PopulateBaseParts(const TArray<UWeaponPartData*>& BaseParts);

	// Called when a base part is clicked
	UFUNCTION()
	void HandleBasePartSelected(UWeaponPartData* SelectedPart);

protected:
	// Bind to this in UMG (named exactly in editor)
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* BasePartGrid;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|UI")
	TSubclassOf<UWeaponPartCardWidget> BasePartCardClass;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|UI")
	TSubclassOf<UWeaponBuilderWidget> BuilderWidgetClass;
};
