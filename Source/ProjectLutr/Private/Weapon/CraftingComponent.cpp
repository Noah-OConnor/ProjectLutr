#include "Weapon/CraftingComponent.h"
#include "Weapon/WeaponPartData.h"
#include "Weapon/WeaponBase.h"

UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AWeaponBase* UCraftingComponent::CraftWeaponFromParts(const TArray<UWeaponPartData*>& Parts)
{
	if (Parts.Num() == 0) return nullptr;

	FWeaponStats FinalStats = CombineStatsFromParts(Parts);

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParams;
	AWeaponBase* NewWeapon = World->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass(), SpawnParams);

	if (NewWeapon)
	{
		NewWeapon->InitializeWeapon(FinalStats, Parts);
	}

	return NewWeapon;
}

FWeaponStats UCraftingComponent::CombineStatsFromParts(const TArray<UWeaponPartData*>& Parts)
{
	FWeaponStats Result;

	for (UWeaponPartData* Part : Parts)
	{
		for (const auto& Stat : Part->StatModifiers)
		{
			// FName StatName = Stat.Key;
			// float Value = Stat.Value;
			//
			// if (StatName == "Damage") Result.Damage += Value;
			// else if (StatName == "Range") Result.Range += Value;
			// else if (StatName == "Accuracy") Result.Accuracy += Value;
			// else if (StatName == "FireRate") Result.FireRate += Value;
			// else if (StatName == "Recoil") Result.Recoil += Value;
			// else if (StatName == "Stability") Result.Stability += Value;
			// else if (StatName == "Capacity") Result.Capacity += Value;
			// else if (StatName == "ProjectileSpeed") Result.ProjectileSpeed += Value;
			// else if (StatName == "ReloadSpeed") Result.ReloadSpeed += Value;
			// else if (StatName == "Piercing") Result.Piercing += Value;
			// else if (StatName == "Weight") Result.Weight += Value;
		}
	}

	return Result;
}
