// Copyright 2025 Project Lutr


#include "Weapon/WeaponInstance.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponDataAsset.h"

void UWeaponInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponInstance, WeaponData);
}

float UWeaponInstance::GetDamage() const
{
	return WeaponData ? WeaponData->BaseDamage : 0.f;
}

float UWeaponInstance::GetFireRate() const
{
	return WeaponData ? WeaponData->FireRate : 0.f;
}

UStaticMesh* UWeaponInstance::GetStaticMesh() const
{
	return WeaponData ? WeaponData->StaticMesh : nullptr;
}

USkeletalMesh* UWeaponInstance::GetSkeletalMesh() const
{
	if (!WeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponData is null in GetStaticMesh()"));
		return nullptr;
	}
	
	return WeaponData ? WeaponData->SkeletalMesh : nullptr;
}