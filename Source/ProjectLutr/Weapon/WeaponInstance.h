// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "WeaponInstance.generated.h"

class UWeaponDataAsset;

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class PROJECTLUTR_API UWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Data", Replicated)
	TObjectPtr<UWeaponDataAsset> WeaponData;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Runtime")
	float CurrentDurability = 100.f;

	// Future: rarity, stat rolls, modifiers, etc.

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	float GetDamage() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	float GetFireRate() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Weapon")
	UStaticMesh* GetStaticMesh() const;
	USkeletalMesh* GetSkeletalMesh() const;
};