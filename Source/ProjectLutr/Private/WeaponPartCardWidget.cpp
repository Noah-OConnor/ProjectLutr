// Copyright 2025 Project Lutr


#include "WeaponPartCardWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ImageUtils.h"
#include "Weapon/WeaponPartData.h"

void UWeaponPartCardWidget::InitializeWithPart(UWeaponPartData* InPart)
{
	PartData = InPart;

	if (PartNameLabel)
	{
		PartNameLabel->SetText(PartData ? PartData->PartName : FText::FromString("Unknown"));
	}

	if (PartData && PartData->PartMesh && PartIconImage)
	{
		UTexture2D* Thumbnail = CaptureThumbnailWithTransparency(PartData->PartMesh);
		if (Thumbnail)
		{
			PartIconImage->SetBrushFromTexture(Thumbnail);
		}
	}

	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &UWeaponPartCardWidget::HandleClick);
	}
}

void UWeaponPartCardWidget::HandleClick()
{
	if (PartData)
	{
		OnClicked.ExecuteIfBound(PartData);
	}
}

UTexture2D* UWeaponPartCardWidget::CaptureThumbnailWithTransparency(UStaticMesh* Mesh)
{
	if (!Mesh) return nullptr;

	// Spawn a preview actor
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	AActor* PreviewActor = World->SpawnActor<AActor>();
	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(PreviewActor);
	MeshComp->RegisterComponent();
	MeshComp->SetStaticMesh(Mesh);
	FVector PreviewOrigin(0.f, 0.f, -100000.f); // isolate
	PreviewActor->SetActorLocation(PreviewOrigin);
	MeshComp->SetWorldLocation(PreviewOrigin);
	PreviewActor->SetRootComponent(MeshComp);

	// Create capture
	ASceneCapture2D* CaptureActor = World->SpawnActor<ASceneCapture2D>();
	USceneCaptureComponent2D* CaptureComp = CaptureActor->GetCaptureComponent2D();
	CaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	CaptureComp->FOVAngle = 20.f;
	CaptureActor->SetActorLocation(PreviewOrigin + FVector(0.f, -200.f, 0.f));
	CaptureComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));


	// Remove unnecessary visual features
	CaptureComp->ShowFlags.SetAtmosphere(false);
	CaptureComp->ShowFlags.SetFog(false);
	CaptureComp->ShowFlags.SetSkyLighting(false);
	CaptureComp->ShowFlags.SetDirectionalLights(false);
	CaptureComp->ShowFlags.SetPointLights(false);
	CaptureComp->ShowFlags.SetSpotLights(false);
	CaptureComp->ShowFlags.SetLighting(false);
	CaptureComp->ShowFlags.SetTranslucency(false);
	CaptureComp->ShowFlags.SetPostProcessing(false);
	CaptureComp->ShowFlags.SetBSP(false);
	CaptureComp->ShowFlags.SetAmbientOcclusion(false);
	CaptureComp->ShowFlags.SetMotionBlur(false);
	CaptureComp->ShowFlags.SetVolumetricFog(false);

	// Set up green-clear render target
	UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>();
	RT->InitAutoFormat(256, 256);
	RT->ClearColor = FLinearColor::Green; // ⬅️ this is your chroma key
	CaptureComp->TextureTarget = RT;

	// Capture it now
	CaptureComp->CaptureScene();

	// Read pixels
	TArray<FColor> Pixels;
	FRenderTarget* RenderTarget = RT->GameThread_GetRenderTargetResource();
	RenderTarget->ReadPixels(Pixels);

	// Convert to Texture2D
	UTexture2D* Texture = UTexture2D::CreateTransient(RT->SizeX, RT->SizeY, PF_B8G8R8A8);
	Texture->AddToRoot(); // prevent garbage collection
	
	Texture->CompressionSettings = TC_Default;
	Texture->SRGB = true;
	Texture->Filter = TF_Default;
	Texture->NeverStream = true;
	Texture->UpdateResource();

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);


	FColor* Dest = static_cast<FColor*>(TextureData);
	for (int32 i = 0; i < Pixels.Num(); i++)
	{
		FColor& Src = Pixels[i];
		if (Src == FColor::Black)
		{
			Src.A = 0;
		}
		else
		{
			Src.A = 255;
		}
		Dest[i] = Src;
	}

	Mip.BulkData.Unlock();
	Texture->UpdateResource();

	// Cleanup
	PreviewActor->Destroy();
	CaptureActor->Destroy();

	return Texture;
}