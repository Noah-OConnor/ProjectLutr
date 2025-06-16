#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UWeaponPartData;
class UWeaponInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLUTR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Inventory")
	void AddWeapon(UWeaponInstance* Weapon);

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Inventory")
	bool RemoveWeapon(UWeaponInstance* Weapon);

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Inventory")
	const TArray<UWeaponInstance*>& GetWeapons() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Inventory")
	UWeaponInstance* GetWeaponAt(int32 Index) const;

protected:
	UPROPERTY(SaveGame, BlueprintReadOnly, Category = "ProjectLutr|Inventory")
	TArray<TObjectPtr<UWeaponInstance>> Weapons;

public:
	UFUNCTION(BlueprintCallable)
	bool AddWeaponPart(UWeaponPartData* NewPart);

	UFUNCTION(BlueprintCallable)
	bool RemoveWeaponPart(UWeaponPartData* PartToRemove);

	UFUNCTION(BlueprintCallable)
	const TArray<UWeaponPartData*>& GetInventory() const;

private:
	UPROPERTY()
	TArray<UWeaponPartData*> Inventory;
		
};
