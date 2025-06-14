#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponPartData.h"
#include "WeaponBuilderWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API UWeaponBuilderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void GeneratePartSlots(const TArray<UWeaponPartData*>& Parts) const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* SlotWrapBox;
};
