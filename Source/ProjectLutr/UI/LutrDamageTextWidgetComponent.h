// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LutrDamageTextWidgetComponent.generated.h"

/**
 * For the floating Damage Numbers when a Character receives damage.
 */
UCLASS()
class PROJECTLUTR_API ULutrDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
