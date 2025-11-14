#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStats.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class UWeaponPartData;

UCLASS()
class PROJECTLUTR_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	void InitializeWeapon(const FWeaponStats& NewStats, const TArray<UWeaponPartData*>& PartsUsed);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly)
	FWeaponStats Stats;

	UPROPERTY(BlueprintReadOnly)
	TArray<UWeaponPartData*> InstalledParts;
};
