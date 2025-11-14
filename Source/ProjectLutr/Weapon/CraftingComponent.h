#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponStats.h"
#include "CraftingComponent.generated.h"

class UWeaponPartData;
class AWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTLUTR_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCraftingComponent();

	UFUNCTION(BlueprintCallable)
	AWeaponBase* CraftWeaponFromParts(const TArray<UWeaponPartData*>& Parts);

protected:
	FWeaponStats CombineStatsFromParts(const TArray<UWeaponPartData*>& Parts);
};
