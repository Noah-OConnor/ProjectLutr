// Copyright 2020 Dan Kestranek.


#include "Player/LutrPlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/LutrPlayerCharacter.h"
#include "Player/LutrPlayerState.h"
#include "UI/LutrDamageTextWidgetComponent.h"
#include "UI/LutrHUDWidget.h"

void ALutrPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<ULutrHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	UIHUDWidget->SetCurrentCapacityReserve(PS->GetCapacityReserve());
	UIHUDWidget->SetMaxCapacityReserve(PS->GetMaxCapacityReserve());
	UIHUDWidget->SetCurrentMana(PS->GetMana());
	UIHUDWidget->SetMaxMana(PS->GetMaxMana());
	UIHUDWidget->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
	UIHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
	UIHUDWidget->SetManaRegenRate(PS->GetManaRegenRate());
	UIHUDWidget->SetCurrentStamina(PS->GetStamina());
	UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
	UIHUDWidget->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	UIHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
	UIHUDWidget->SetExperience(PS->GetXP());
	UIHUDWidget->SetGold(PS->GetGold());
	UIHUDWidget->SetHeroLevel(PS->GetCharacterLevel());
}

ULutrHUDWidget * ALutrPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void ALutrPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ALutrCharacterBase* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		ULutrDamageTextWidgetComponent* DamageText = NewObject<ULutrDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool ALutrPlayerController::ShowDamageNumber_Validate(float DamageAmount, ALutrCharacterBase* TargetCharacter)
{
	return true;
}

void ALutrPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
	}
}

bool ALutrPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

// Server only
void ALutrPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ALutrPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}
