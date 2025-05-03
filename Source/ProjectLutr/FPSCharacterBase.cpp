// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacterBase.h"
#include "PlayerAttributeSetBase.h"
#include "FPSCharacterMovementComponent.h"
#include "FPSAbilitySystemComponent.h"
#include "FPSGameplayAbility.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFPSCharacterBase::AFPSCharacterBase(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
}

UAbilitySystemComponent* AFPSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AFPSCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;;
}

//void AFPSCharacterBase::RemoveCharacterAbilities()
//{
//}

float AFPSCharacterBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AFPSCharacterBase::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

// Called when the game starts or when spawned
void AFPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UFPSGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}
//
//void AFPSCharacterBase::InitializeAttributes()
//{
//}
//
//void AFPSCharacterBase::AddStartupEffects()
//{
//}

void AFPSCharacterBase::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

