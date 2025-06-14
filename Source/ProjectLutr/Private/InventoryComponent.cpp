#include "InventoryComponent.h"
#include "Weapon/WeaponPartData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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
