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
	TMap<FName, float> StatModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RarityTier;
};