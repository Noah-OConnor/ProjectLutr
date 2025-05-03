// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "PlayerAttributeSet.h"
#include "FPSCharacterMovementComponent.h"
#include "FPSAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
}

UAbilitySystemComponent * AFPSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AFPSCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;;
}

//void AFPSCharacter::RemoveCharacterAbilities()
//{
//}

float AFPSCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AFPSCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//void AFPSCharacter::AddCharacterAbilities()
//{
//
//}
//
//void AFPSCharacter::InitializeAttributes()
//{
//}
//
//void AFPSCharacter::AddStartupEffects()
//{
//}

void AFPSCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

