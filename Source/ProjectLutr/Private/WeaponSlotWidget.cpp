
#include "WeaponSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UWeaponSlotWidget::InitializeSlot(const EWeaponPartType SlotType)
{
	if (SlotTypeLabel)
	{
		// Use DisplayName override or fallback to enum string
		FText LabelText = FText::FromString(UEnum::GetValueAsString(SlotType).RightChop(18)); // remove "EWeaponPartType::"

		SlotTypeLabel->SetText(LabelText);
	}

	if (PartIcon)
	{
		// if (SlotData.Icon)
		// {
		// 	FSlateBrush Brush;
		// 	Brush.SetResourceObject(SlotData.Icon);
		// 	PartIcon->SetBrush(Brush);
		// }
		// else
		// {
		// 	PartIcon->SetBrushFromTexture(nullptr); // or a default icon
		// }
	}

	if (PartNameLabel)
	{
		PartNameLabel->SetText(FText::FromString("Empty")); // Can be replaced later
	}
}
