#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UWeaponPartData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLUTR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Add a weapon part to the inventory
	UFUNCTION(BlueprintCallable)
	bool AddWeaponPart(UWeaponPartData* NewPart);

	// Remove a part (if needed)
	UFUNCTION(BlueprintCallable)
	bool RemoveWeaponPart(UWeaponPartData* PartToRemove);

	// Get all weapon parts
	UFUNCTION(BlueprintCallable)
	const TArray<UWeaponPartData*>& GetInventory() const;

private:
	UPROPERTY()
	TArray<UWeaponPartData*> Inventory;
		
};
