// Copyright 2023 Dan Kestranek.


#include "WeaponBuilderWidget.h"

#include "WeaponSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UWeaponBuilderWidget::GeneratePartSlots(const TArray<UWeaponPartData*>& Parts) const
{
	if (!SlotWrapBox) return;
	SlotWrapBox->ClearChildren();

	for (const UWeaponPartData* Part : Parts)
	{
		if (!Part) continue;
		for (const EWeaponPartType PartSlot : Part->ChildSlots)
		{
			UWeaponSlotWidget* SlotWidget = nullptr;//CreateWidget<UWeaponSlotWidget(this, SlotWidget);
			if (!SlotWidget) continue;

			SlotWidget->InitializeSlot(PartSlot);

			UWrapBoxSlot* WrapSlot = SlotWrapBox->AddChildToWrapBox(SlotWidget);
			WrapSlot->SetPadding(FMargin(5.0f));
		}
	}
}
