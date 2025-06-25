#include "Weapon/WeaponBase.h"
#include "Components/StaticMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = Mesh;
}

void AWeaponBase::InitializeWeapon(const FWeaponStats& NewStats, const TArray<UWeaponPartData*>& PartsUsed)
{
	Stats = NewStats;
	InstalledParts = PartsUsed;

	// TODO: Apply mesh customization based on parts
	// TODO: Apply stat-driven gameplay behavior
}
