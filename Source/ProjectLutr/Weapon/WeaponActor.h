// Copyright 2025 Project Lutr

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UWeaponDataAsset;
class UStaticMeshComponent;

UCLASS()
class PROJECTLUTR_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMesh;

	void SetWeaponInstance(class UWeaponInstance* WeaponInstance);

	UPROPERTY(ReplicatedUsing = OnRep_WeaponData)
	UWeaponDataAsset* ReplicatedWeaponData;

	UFUNCTION()
	void OnRep_WeaponData();
	void ApplyWeaponData(UWeaponDataAsset* Data);
};
