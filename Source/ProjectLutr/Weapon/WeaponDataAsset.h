// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class UGameplayAbility;

UCLASS(BlueprintType)
class PROJECTLUTR_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Mesh")
	USkeletalMesh* SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Ability")
	TSubclassOf<UGameplayAbility> FireAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Stats")
	float BaseDamage = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Stats")
	float FireRate = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Stats")
	float Range = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectLutr|Tags")
	FGameplayTagContainer WeaponTags;
};