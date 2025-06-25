// Copyright 2020 Dan Kestranek.


#include "UI/LutrFloatingStatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULutrFloatingStatusBarWidget::SetHealthPercentage(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

void ULutrFloatingStatusBarWidget::SetCharacterName(const FText& NewName)
{
	if (NameText)
	{
		NameText->SetText(NewName);
	}
}
