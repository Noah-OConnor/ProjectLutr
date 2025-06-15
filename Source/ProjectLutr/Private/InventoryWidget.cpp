// Copyright 2025 Project Lutr


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Weapon/WeaponPartData.h"
#include "WeaponPartCardWidget.h"

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
	// TODO: Open Weapon Builder with this base part
	UE_LOG(LogTemp, Warning, TEXT("Selected Base: %s"), *SelectedPart->PartName.ToString());
}
