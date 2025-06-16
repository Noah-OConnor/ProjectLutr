// Copyright 2025 Project Lutr


#include "UI/Inventory/InventoryWidget.h"

#include "UI/Inventory/WeaponBuilderWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Weapon/WeaponPartData.h"
#include "UI/Inventory/WeaponPartCardWidget.h"

void UInventoryWidget::PopulateBaseParts(const TArray<UWeaponPartData*>& BaseParts)
{
	if (!BasePartGrid || !BasePartCardClass) return;

	BasePartGrid->ClearChildren();

	const int32 NumColumns = 4;
	int32 Index = 0;

	for (UWeaponPartData* Part : BaseParts)
	{
		UWeaponPartCardWidget* Card = CreateWidget<UWeaponPartCardWidget>(this, BasePartCardClass);
		if (!Card) continue;

		Card->InitializeWithPart(Part);
		Card->OnClicked.BindUObject(this, &UInventoryWidget::HandleBasePartSelected);

		int32 Row = Index / NumColumns;
		int32 Col = Index % NumColumns;

		BasePartGrid->AddChildToUniformGrid(Card, Row, Col);
		++Index;
	}
}

void UInventoryWidget::HandleBasePartSelected(UWeaponPartData* SelectedPart)
{
	if (!SelectedPart) return;

	// Create builder widget
	if (BuilderWidgetClass)
	{
		UWeaponBuilderWidget* BuilderWidget = CreateWidget<UWeaponBuilderWidget>(GetOwningPlayer(), BuilderWidgetClass);
		if (BuilderWidget)
		{
			// Pass the base part to initialize the builder
			BuilderWidget->InitializeFromBase(SelectedPart);

			// Remove the inventory UI
			RemoveFromParent();

			// Show builder
			BuilderWidget->AddToViewport();

			// Set input mode (if needed)
			APlayerController* PC = GetOwningPlayer();
			if (PC)
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(BuilderWidget->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
	}
}

