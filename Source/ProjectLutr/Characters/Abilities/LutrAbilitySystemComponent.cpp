// Copyright 2020 Dan Kestranek.


#include "ProjectLutr/Characters/Abilities/LutrAbilitySystemComponent.h"

void ULutrAbilitySystemComponent::ReceiveDamage(ULutrAbilitySystemComponent * SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
