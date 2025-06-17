// Copyright 2025 Project Lutr


#include "Characters/Player/Abilities/LutrGA_Jump.h"
#include "GameFramework/Character.h"

ULutrGA_Jump::ULutrGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	AbilityInputID = EAbilityInputID::Jump;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void ULutrGA_Jump::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Display, TEXT("ULutrGA_Jump::ActivateAbility"));
	
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character && Character->CanJump())
	{
		Character->Jump();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void ULutrGA_Jump::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->StopJumping();
	}
}

bool ULutrGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, OptionalRelevantTags))
		return false;

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return Character && Character->CanJump();
}
