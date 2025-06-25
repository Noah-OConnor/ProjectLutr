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

	UFUNCTION(BlueprintCallable)
	void AssembleWeaponFromParts(const TArray<UWeaponPartData*>& Parts);

	UTextureRenderTarget2D* GetPreviewRenderTarget() const { return RenderTarget; }

protected:
	UPROPERTY()
	TMap<EWeaponPartType, UStaticMeshComponent*> MeshComponents;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Preview")
	TSubclassOf<UStaticMeshComponent> MeshComponentClass;

	UPROPERTY()
	USceneCaptureComponent2D* CaptureComponent;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	void ClearPreviousParts();
	void AttachMeshPart(EWeaponPartType PartType, UStaticMesh* Mesh);
};
