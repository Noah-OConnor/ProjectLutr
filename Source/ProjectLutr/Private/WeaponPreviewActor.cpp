// Copyright 2025 Project Lutr


#include "WeaponPreviewActor.h"
#include "Components/StaticMeshComponent.h"

AWeaponPreviewActor::AWeaponPreviewActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(false);
}

void AWeaponPreviewActor::ClearPreviousParts()
{
	for (auto& Pair : MeshComponents)
	{
		if (Pair.Value)
		{
			Pair.Value->DestroyComponent();
		}
	}
	MeshComponents.Empty();
}

void AWeaponPreviewActor::AssembleWeaponFromParts(const TArray<UWeaponPartData*>& Parts)
{
	ClearPreviousParts();

	for (UWeaponPartData* Part : Parts)
	{
		if (!Part || !Part->PartMesh) continue;

		AttachMeshPart(Part->PartType, Part->PartMesh);
	}
}

void AWeaponPreviewActor::AttachMeshPart(EWeaponPartType PartType, UStaticMesh* Mesh)
{
	if (!Mesh) return;

	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this);
	MeshComp->RegisterComponent();
	MeshComp->SetStaticMesh(Mesh);
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// First part (usually Base) becomes root
	if (MeshComponents.Num() == 0)
	{
		RootComponent = MeshComp;
	}
	else
	{
		MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	MeshComponents.Add(PartType, MeshComp);
}
