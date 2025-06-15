// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/WeaponPartData.h"
#include "WeaponPreviewActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECTLUTR_API AWeaponPreviewActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponPreviewActor();

	// Main entry to build the weapon from part data
	UFUNCTION(BlueprintCallable)
	void AssembleWeaponFromParts(const TArray<UWeaponPartData*>& Parts);

protected:
	UPROPERTY()
	TMap<EWeaponPartType, UStaticMeshComponent*> MeshComponents;

	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TSubclassOf<UStaticMeshComponent> MeshComponentClass;

	void ClearPreviousParts();
	void AttachMeshPart(EWeaponPartType PartType, UStaticMesh* Mesh);
};
