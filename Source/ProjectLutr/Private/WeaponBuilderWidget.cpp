#include "WeaponBuilderWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/TextBlock.h"
#include "WeaponSlotWidget.h"
#include "WeaponPreviewActor.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"

void UWeaponBuilderWidget::InitializeFromBase(UWeaponPartData* InBasePart)
{
	if (!InBasePart) return;

	BasePart = InBasePart;
	EquippedParts.Empty();
	EquippedParts.Add(EWeaponPartType::Base, BasePart);

	SpawnOrUpdatePreview();
	GeneratePartSlots();
	UpdateStatsPanel();
}

void UWeaponBuilderWidget::GeneratePartSlots()
{
	if (!SlotWrapBox || !SlotWidgetClass || !BasePart) return;

	SlotWrapBox->ClearChildren();

	for (const EWeaponPartType SlotType : BasePart->ChildSlots)
	{
		UWeaponSlotWidget* SlotWidget = CreateWidget<UWeaponSlotWidget>(this, SlotWidgetClass);
		if (!SlotWidget) continue;

		SlotWidget->InitializeSlot(SlotType, EquippedParts.FindRef(SlotType));
		SlotWidget->OnSlotClicked.BindUFunction(this, FName("HandleSlotClicked"));

		UWrapBoxSlot* WrapSlot = SlotWrapBox->AddChildToWrapBox(SlotWidget);
		WrapSlot->SetPadding(FMargin(5.f));
	}
}

void UWeaponBuilderWidget::UpdateStatsPanel()
{
	float Damage = 0.f;
	float Accuracy = 0.f;
	float Recoil = 0.f;

	for (const auto& Pair : EquippedParts)
	{
		const UWeaponPartData* Part = Pair.Value;
		if (!Part) continue;

		Damage += Part->StatModifiers.FindRef(EWeaponStatType::Damage);
		Accuracy += Part->StatModifiers.FindRef(EWeaponStatType::Accuracy);
		Recoil += Part->StatModifiers.FindRef(EWeaponStatType::Recoil);
	}

	if (StatDamage) StatDamage->SetText(FText::AsNumber(Damage));
	if (StatAccuracy) StatAccuracy->SetText(FText::AsNumber(Accuracy));
	if (StatRecoil) StatRecoil->SetText(FText::AsNumber(Recoil));
}

void UWeaponBuilderWidget::SpawnOrUpdatePreview()
{
	if (!BasePart) return;

	if (!PreviewActor)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector SpawnLoc(0.f, 0.f, 100.f);
		FRotator SpawnRot(0.f, -90.f, 0.f);

		PreviewActor = GetWorld()->SpawnActor<AWeaponPreviewActor>(AWeaponPreviewActor::StaticClass(), SpawnLoc, SpawnRot, SpawnParams);
	}

	if (PreviewActor)
	{
		TArray<UWeaponPartData*> Parts;
		for (auto& Pair : EquippedParts)
		{
			if (Pair.Value)
				Parts.Add(Pair.Value);
		}
		PreviewActor->AssembleWeaponFromParts(Parts);

		UTextureRenderTarget2D* RenderTarget = PreviewActor->GetPreviewRenderTarget();
		if (RenderTarget && PreviewImage)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(RenderTarget);
			Brush.ImageSize = FVector2D(512.f, 512.f);
			PreviewImage->SetBrush(Brush);
		}
	}
}

void UWeaponBuilderWidget::HandleSlotClicked(EWeaponPartType SlotType)
{
	// This would open a popup with a filtered list of compatible parts
	// For now, log the intent
	UE_LOG(LogTemp, Log, TEXT("Slot clicked: %d"), (int32)SlotType);
}
