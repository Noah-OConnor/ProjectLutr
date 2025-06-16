// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponPartCardWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UWeaponPartData;

DECLARE_DELEGATE_OneParam(FOnPartCardClicked, UWeaponPartData*)

UCLASS()
class PROJECTLUTR_API UWeaponPartCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Call this right after CreateWidget
	void InitializeWithPart(UWeaponPartData* InPart);

	// Delegate for when this card is clicked
	FOnPartCardClicked OnClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PartNameLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* PartIconImage;

	UFUNCTION()
	void HandleClick();

	UPROPERTY()
	UWeaponPartData* PartData;

	UTexture2D* CaptureThumbnailWithTransparency(UStaticMesh* Mesh);

};


