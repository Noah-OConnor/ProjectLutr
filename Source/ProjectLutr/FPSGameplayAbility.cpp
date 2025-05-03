// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameplayAbility.h"

UFPSGameplayAbility::UFPSGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFPSGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
}
