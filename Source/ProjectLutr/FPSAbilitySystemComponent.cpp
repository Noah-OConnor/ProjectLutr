// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAbilitySystemComponent.h"

void UFPSAbilitySystemComponent::ReceiveDamage(UFPSAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}