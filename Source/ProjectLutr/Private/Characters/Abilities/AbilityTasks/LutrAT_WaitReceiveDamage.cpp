// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/LutrAT_WaitReceiveDamage.h"
#include "Characters/Abilities/LutrAbilitySystemComponent.h"

ULutrAT_WaitReceiveDamage::ULutrAT_WaitReceiveDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TriggerOnce = false;
}

ULutrAT_WaitReceiveDamage* ULutrAT_WaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	ULutrAT_WaitReceiveDamage* MyObj = NewAbilityTask<ULutrAT_WaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void ULutrAT_WaitReceiveDamage::Activate()
{
	ULutrAbilitySystemComponent* GDASC = Cast<ULutrAbilitySystemComponent>(AbilitySystemComponent);

	if (GDASC)
	{
		GDASC->ReceivedDamage.AddDynamic(this, &ULutrAT_WaitReceiveDamage::OnDamageReceived);
	}
}

void ULutrAT_WaitReceiveDamage::OnDestroy(bool AbilityIsEnding)
{
	ULutrAbilitySystemComponent* GDASC = Cast<ULutrAbilitySystemComponent>(AbilitySystemComponent);

	if (GDASC)
	{
		GDASC->ReceivedDamage.RemoveDynamic(this, &ULutrAT_WaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void ULutrAT_WaitReceiveDamage::OnDamageReceived(ULutrAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}
