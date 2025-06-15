#include "WeaponSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UWeaponSlotWidget::InitializeSlot(EWeaponPartType InSlotType, UWeaponPartData* EquippedPart)
{
	SlotType = InSlotType;
	CurrentPart = EquippedPart;

	if (SlotTypeLabel)
	{
		FString EnumString = UEnum::GetValueAsString(SlotType);
		SlotTypeLabel->SetText(FText::FromString(EnumString.RightChop(18))); // Strip "EWeaponPartType::"
	}

	if (PartNameLabel)
	{
		PartNameLabel->SetText(CurrentPart ? CurrentPart->PartName : FText::FromString("Empty"));
	}

	if (PartIcon)
	{
		if (CurrentPart && CurrentPart->PartMesh)
		{
			// Optional: generate thumbnail dynamically
			PartIcon->SetBrushFromTexture(nullptr); // placeholder
		}
		else
		{
			PartIcon->SetBrushFromTexture(nullptr);
		}
	}

	if (SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &UWeaponSlotWidget::HandleClick);
	}
}

void UWeaponSlotWidget::HandleClick()
{
	OnSlotClicked.ExecuteIfBound(SlotType);
}