// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerState.h"
#include "FPSAbilitySystemComponent.h"
#include "PlayerAttributeSet.h"

AFPSPlayerState::AFPSPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
    AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    // Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
    // we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create the attribute set, this replicates by default
    // Adding it as a subobject of the owning actor of an AbilitySystemComponent
    // automatically registers the AttributeSet with the AbilitySystemComponent
    AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

    // Set the network update frequency for better responsiveness
    SetNetUpdateFrequency(30.0f);
}

UAbilitySystemComponent* AFPSPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UPlayerAttributeSet* AFPSPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

bool AFPSPlayerState::IsAlive() const
{
    return GetHealth() > 0.0f;
}

float AFPSPlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float AFPSPlayerState::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}
