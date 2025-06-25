#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponPartData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponPartType : uint8
{
	Base,
	Barrel,
	Magazine,
	Muzzle,
	Sight,
	Stock,
	Grip,
	Accessory,
	Underbarrel
};

UENUM(BlueprintType)
enum class EWeaponStatType : uint8
{
	Damage,
	Range,
	Accuracy,
	FireRate,
	Recoil,
	Weight,
	Capacity,
	ProjectileSpeed,
	ReloadSpeed,
	RechargeRate,
	Stability,
	Piercing,
	AmmoType
};

/**
 * 
 */

UCLASS(BlueprintType)
class PROJECTLUTR_API UWeaponPartData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponPartType PartType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PartName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* PartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EWeaponStatType, float> StatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Slot System")
	TArray<EWeaponPartType> ChildSlots;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// int32 RarityTier;
};