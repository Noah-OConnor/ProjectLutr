// Copyright 2025 Project Lutr


#include "Player/LutrPlayerState.h"
#include "Characters/Abilities/AttributeSets/LutrAttributeSetBase.h"
#include "Characters/Abilities/LutrAbilitySystemComponent.h"
#include "Characters/Components/InventoryComponent.h"
#include "Characters/Player/LutrPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Player/LutrPlayerController.h"
#include "UI/LutrFloatingStatusBarWidget.h"
#include "UI/LutrHUDWidget.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponInstance.h"

ALutrPlayerState::ALutrPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<ULutrAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<ULutrAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	SetNetUpdateFrequency(100.0f);

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent * ALutrPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

ULutrAttributeSetBase * ALutrPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ALutrPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ALutrPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->ShowAbilityConfirmCancelText(ShowText);
		}
	}
}

float ALutrPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ALutrPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ALutrPlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float ALutrPlayerState::GetCapacityReserve() const
{
	return AttributeSetBase->GetCapacityReserve();
}

float ALutrPlayerState::GetMaxCapacityReserve() const
{
	return AttributeSetBase->GetMaxCapacityReserve();
}

float ALutrPlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ALutrPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

float ALutrPlayerState::GetManaRegenRate() const
{
	return AttributeSetBase->GetManaRegenRate();
}

float ALutrPlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float ALutrPlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float ALutrPlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

float ALutrPlayerState::GetArmor() const
{
	return AttributeSetBase->GetArmor();
}

float ALutrPlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

int32 ALutrPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

int32 ALutrPlayerState::GetXP() const
{
	return AttributeSetBase->GetXP();
}

int32 ALutrPlayerState::GetXPBounty() const
{
	return AttributeSetBase->GetXPBounty();
}

int32 ALutrPlayerState::GetGold() const
{
	return AttributeSetBase->GetGold();
}

int32 ALutrPlayerState::GetGoldBounty() const
{
	return AttributeSetBase->GetGoldBounty();
}

void ALutrPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ALutrPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ALutrPlayerState::MaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &ALutrPlayerState::HealthRegenRateChanged);
		CapacityReserveChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCapacityReserveAttribute()).AddUObject(this, &ALutrPlayerState::CapacityReserveChanged);
		MaxCapacityReserveChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxCapacityReserveAttribute()).AddUObject(this, &ALutrPlayerState::MaxCapacityReserveChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject(this, &ALutrPlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute()).AddUObject(this, &ALutrPlayerState::MaxManaChanged);
		ManaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaRegenRateAttribute()).AddUObject(this, &ALutrPlayerState::ManaRegenRateChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ALutrPlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &ALutrPlayerState::MaxStaminaChanged);
		StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &ALutrPlayerState::StaminaRegenRateChanged);
		XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetXPAttribute()).AddUObject(this, &ALutrPlayerState::XPChanged);
		GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetGoldAttribute()).AddUObject(this, &ALutrPlayerState::GoldChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &ALutrPlayerState::CharacterLevelChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ALutrPlayerState::StunTagChanged);
	}
}

void ALutrPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ALutrPlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	float Health = Data.NewValue;

	// Update floating status bar
	ALutrPlayerCharacter* Hero = Cast<ALutrPlayerCharacter>(GetPawn());
	if (Hero)
	{
		ULutrFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
		}
	}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	// If the player died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (Hero)
		{
			Hero->Die();
		}
	}
}

void ALutrPlayerState::MaxHealthChanged(const FOnAttributeChangeData & Data)
{
	float MaxHealth = Data.NewValue;

	// Update floating status bar
	ALutrPlayerCharacter* Hero = Cast<ALutrPlayerCharacter>(GetPawn());
	if (Hero)
	{
		ULutrFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxHealth(MaxHealth);
		}
	}
}

void ALutrPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float HealthRegenRate = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetHealthRegenRate(HealthRegenRate);
		}
	}
}

void ALutrPlayerState::CapacityReserveChanged(const FOnAttributeChangeData& Data)
{
	// nothing here since we don't need to update the floating hud
}

void ALutrPlayerState::MaxCapacityReserveChanged(const FOnAttributeChangeData& Data)
{
}

void ALutrPlayerState::ManaChanged(const FOnAttributeChangeData & Data)
{
	float Mana = Data.NewValue;

	// Update floating status bar
	ALutrPlayerCharacter* Hero = Cast<ALutrPlayerCharacter>(GetPawn());
	if (Hero)
	{
		ULutrFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			//HeroFloatingStatusBar->SetManaPercentage(Mana / GetMaxMana());
		}
	}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
}

void ALutrPlayerState::MaxManaChanged(const FOnAttributeChangeData & Data)
{
	float MaxMana = Data.NewValue;

	// Update floating status bar
	ALutrPlayerCharacter* Hero = Cast<ALutrPlayerCharacter>(GetPawn());
	if (Hero)
	{
		ULutrFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			//HeroFloatingStatusBar->SetManaPercentage(GetMana() / MaxMana);
		}
	}

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxMana(MaxMana);
		}
	}
}

void ALutrPlayerState::ManaRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float ManaRegenRate = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetManaRegenRate(ManaRegenRate);
		}
	}
}

void ALutrPlayerState::StaminaChanged(const FOnAttributeChangeData & Data)
{
	float Stamina = Data.NewValue;

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
}

void ALutrPlayerState::MaxStaminaChanged(const FOnAttributeChangeData & Data)
{
	float MaxStamina = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxStamina(MaxStamina);
		}
	}
}

void ALutrPlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float StaminaRegenRate = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetStaminaRegenRate(StaminaRegenRate);
		}
	}
}

void ALutrPlayerState::XPChanged(const FOnAttributeChangeData & Data)
{
	float XP = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetExperience(XP);
		}
	}
}

void ALutrPlayerState::GoldChanged(const FOnAttributeChangeData & Data)
{
	float Gold = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetGold(Gold);
		}
	}
}

void ALutrPlayerState::CharacterLevelChanged(const FOnAttributeChangeData & Data)
{
	float CharacterLevel = Data.NewValue;

	// Update the HUD
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetOwner());
	if (PC)
	{
		ULutrHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetHeroLevel(CharacterLevel);
		}
	}
}

void ALutrPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
