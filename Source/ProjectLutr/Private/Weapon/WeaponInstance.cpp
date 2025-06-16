// Copyright 2025 Project Lutr


#include "Weapon/WeaponInstance.h"
#include "Weapon/WeaponDataAsset.h"

float UWeaponInstance::GetDamage() const
{
	return WeaponData ? WeaponData->BaseDamage : 0.f;
}

float UWeaponInstance::GetFireRate() const
{
	return WeaponData ? WeaponData->FireRate : 0.f;
}

UStaticMesh* UWeaponInstance::GetMesh() const
{
	return WeaponData ? WeaponData->WeaponMesh : nullptr;
}