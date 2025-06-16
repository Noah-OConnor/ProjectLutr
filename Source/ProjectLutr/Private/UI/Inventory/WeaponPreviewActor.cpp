// Copyright 2025 Project Lutr


#include "UI/Inventory/WeaponPreviewActor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"

AWeaponPreviewActor::AWeaponPreviewActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(false);

	// Create capture component
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	CaptureComponent->SetupAttachment(RootComponent);
	CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	CaptureComponent->FOVAngle = 25.f;
	//CaptureComponent->ShowOnlyComponents.Empty();

	CaptureComponent->ShowFlags.SetAtmosphere(false);
	//CaptureComponent->ShowFlags.SetSkyLighting(false);
	//CaptureComponent->ShowFlags.SetFog(false);
	//CaptureComponent->ShowFlags.SetPostProcessing(false);

	
	// Create and assign render target
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(1920, 500);
	RenderTarget->ClearColor = FLinearColor::Black;
	RenderTarget->TargetGamma = 2.2f;
	RenderTarget->UpdateResource();

	CaptureComponent->TextureTarget = RenderTarget;

	// Set capture distance (we'll refine this later)
	CaptureComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CaptureComponent->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));
}

void AWeaponPreviewActor::ClearPreviousParts()
{
	if (CaptureComponent)
	{
		CaptureComponent->ShowOnlyComponents.Empty();
	}
	
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

	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(true, Origin, BoxExtent);

	// Offset the whole root
	if (RootComponent)
	{
		RootComponent->SetRelativeLocation(-Origin); // Brings the mesh center to (0,0,0)
	}

}

void AWeaponPreviewActor::AttachMeshPart(EWeaponPartType PartType, UStaticMesh* Mesh)
{
	if (!Mesh || !CaptureComponent) return;
	
	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this);
	MeshComp->RegisterComponent();
	MeshComp->SetStaticMesh(Mesh);
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Position the mesh at the actor's origin
	MeshComp->SetRelativeLocation(FVector::ZeroVector);
	MeshComp->SetRelativeRotation(FRotator::ZeroRotator);

	// Attach to root or create one
	if (MeshComponents.Num() == 0)
	{
		RootComponent = MeshComp;
	}
	else
	{
		MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}


	MeshComponents.Add(PartType, MeshComp);
	CaptureComponent->ShowOnlyComponents.Add(MeshComp);
}
