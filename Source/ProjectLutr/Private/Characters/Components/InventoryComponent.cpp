#include "Characters/Components/InventoryComponent.h"
#include "Weapon/WeaponPartData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::AddWeapon(UWeaponInstance* Weapon)
{
	if (Weapon && !Weapons.Contains(Weapon))
	{
		Weapons.Add(Weapon);
	}
}

bool UInventoryComponent::RemoveWeapon(UWeaponInstance* Weapon)
{
	return Weapons.Remove(Weapon) > 0;
}

const TArray<UWeaponInstance*>& UInventoryComponent::GetWeapons() const
{
	return Weapons;
}

UWeaponInstance* UInventoryComponent::GetWeaponAt(int32 Index) const
{
	return Weapons.IsValidIndex(Index) ? Weapons[Index] : nullptr;
}

bool UInventoryComponent::AddWeaponPart(UWeaponPartData* NewPart)
{
	if (!NewPart) return false;

	Inventory.Add(NewPart);
	return true;
}

bool UInventoryComponent::RemoveWeaponPart(UWeaponPartData* PartToRemove)
{
	return Inventory.Remove(PartToRemove) > 0;
}

const TArray<UWeaponPartData*>& UInventoryComponent::GetInventory() const
{
	return Inventory;
}
