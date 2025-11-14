#pragma once

#include "WeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stability = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Capacity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Piercing = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 0.0f;
};
