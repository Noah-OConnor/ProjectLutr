#include "Characters/Components/InventoryComponent.h"

#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponPartData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, Weapons);
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
	UE_LOG(LogTemp, Warning, TEXT("Inventory contains %d weapons"), Weapons.Num());

	if (Weapons.IsValidIndex(Index))
	{
		return Weapons[Index];
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon index %d out of range"), Index);
	return nullptr;
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
