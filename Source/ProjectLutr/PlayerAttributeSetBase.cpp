// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAttributeSetBase.h"
#include "Net/UnrealNetwork.h"
#include "FPSCharacterBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "FPSPlayerController.h"

void UPlayerAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UPlayerAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AFPSCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AFPSCharacterBase>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AFPSCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<AFPSCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AFPSCharacterBase>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			if (!TargetCharacter->IsAlive())
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), TEXT(__FUNCTION__), *TargetCharacter->GetName());
			}

			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

			if (TargetCharacter && WasAlive)
			{
				// This is the log statement for damage received. Turned off for live games.
				//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

				// Play HitReact animation and sound with a multicast RPC.
				const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

				//if (Hit)
				//{
				//	EGDHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(Data.EffectSpec.GetContext().GetHitResult()->Location);
				//	switch (HitDirection)
				//	{
				//	case EGDHitReactDirection::Left:
				//		TargetCharacter->PlayHitReact(HitDirectionLeftTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Front:
				//		TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Right:
				//		TargetCharacter->PlayHitReact(HitDirectionRightTag, SourceCharacter);
				//		break;
				//	case EGDHitReactDirection::Back:
				//		TargetCharacter->PlayHitReact(HitDirectionBackTag, SourceCharacter);
				//		break;
				//	}
				//}
				//else
				//{
				//	// No hit result. Default to front.
				//	TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				//}

				// Show damage number for the Source player unless it was self damage
				if (SourceActor != TargetActor)
				{
					AFPSPlayerController* PC = Cast<AFPSPlayerController>(SourceController);
					if (PC)
					{
						//PC->ShowDamageNumber(LocalDamageDone, TargetCharacter);
					}
				}

				//if (!TargetCharacter->IsAlive())
				//{
				//	// TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
				//	// Don't give bounty to self.
				//	if (SourceController != TargetController)
				//	{
				//		// Create a dynamic instant Gameplay Effect to give the bounties
				//		UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
				//		GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

				//		int32 Idx = GEBounty->Modifiers.Num();
				//		GEBounty->Modifiers.SetNum(Idx + 2);

				//		FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
				//		InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
				//		InfoXP.ModifierOp = EGameplayModOp::Additive;
				//		InfoXP.Attribute = UGDAttributeSetBase::GetXPAttribute();

				//		FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
				//		InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
				//		InfoGold.ModifierOp = EGameplayModOp::Additive;
				//		InfoGold.Attribute = UGDAttributeSetBase::GetGoldAttribute();

				//		Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
				//	}
				//}
			}
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	} // Health
}

void UPlayerAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSetBase::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UPlayerAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSetBase, Health, OldHealth);
}

void UPlayerAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSetBase, MaxHealth, OldMaxHealth);
}