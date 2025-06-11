// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/LutrCharacterBase.h"
#include "LutrPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ALutrPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void CreateHUD();

	UPROPERTY(EditAnywhere, Category = "ProjectLutr|UI")
	TSubclassOf<class ULutrDamageTextWidgetComponent> DamageNumberClass;

	class ULutrHUDWidget* GetHUD();

	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, ALutrCharacterBase* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, ALutrCharacterBase* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, ALutrCharacterBase* TargetCharacter);

	// Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the Server.
	UFUNCTION(Client, Reliable, WithValidation)
	void SetRespawnCountdown(float RespawnTimeRemaining);
	void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
	bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|UI")
	TSubclassOf<class ULutrHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectLutr|UI")
	class ULutrHUDWidget* UIHUDWidget;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
