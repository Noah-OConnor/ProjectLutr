// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "FPSPlayerState.h"
#include "AbilitySystemComponent.h"

void AFPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AFPSPlayerState* PS = GetPlayerState<AFPSPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		//PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AFPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
