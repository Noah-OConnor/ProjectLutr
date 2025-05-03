// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSCharacterBase.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//void CreateHUD();

	/*UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGDDamageTextWidgetComponent> DamageNumberClass;*/

	//class UFPSHUDWidget* GetHUD();

protected:

	//Server Only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
