#include "WeaponBuilderWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/TextBlock.h"
#include "WeaponSlotWidget.h"
#include "WeaponPreviewActor.h"
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
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponPreviewActor::StaticClass(), Found);
		if (Found.Num() > 0)
		{
			PreviewActor = Cast<AWeaponPreviewActor>(Found[0]);
		}
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
	}
}

void UWeaponBuilderWidget::HandleSlotClicked(EWeaponPartType SlotType)
{
	// This would open a popup with a filtered list of compatible parts
	// For now, log the intent
	UE_LOG(LogTemp, Log, TEXT("Slot clicked: %d"), (int32)SlotType);
}
