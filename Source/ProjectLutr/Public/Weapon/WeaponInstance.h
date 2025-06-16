// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "WeaponInstance.generated.h"

class UWeaponDataAsset;

UCLASS(BlueprintType)
class PROJECTLUTR_API UWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Data")
	TObjectPtr<UWeaponDataAsset> WeaponData;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Runtime")
	float CurrentDurability = 100.f;

	// Future: rarity, stat rolls, modifiers, etc.

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	float GetDamage() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	float GetFireRate() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	UStaticMesh* GetMesh() const;
};