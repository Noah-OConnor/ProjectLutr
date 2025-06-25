// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "LutrPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLUTR_API ALutrPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
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
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputMappingContext* PlayerMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* ADSAction;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* EquipWeaponAction;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* FireWeaponAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|Input")
	UInputAction* OpenInventoryAction;
	
	UFUNCTION()
	void HandleMove(const FInputActionValue& Value);
	UFUNCTION()
	void HandleLook(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();
	UFUNCTION()
	void HandleJumpPressed();
	UFUNCTION()
	void HandleJumpReleased();
	UFUNCTION()
	void StartADS();
	UFUNCTION()
	void StopADS();
	UFUNCTION()
	void HandleEquipWeaponPressed();
	UFUNCTION()
	void HandleFireWeaponPressed();
	UFUNCTION()
	void HandleFireWeaponReleased();
	UFUNCTION()
	void HandleOpenInventory(const FInputActionValue& ActionValue);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|UI")
	TSubclassOf<class ULutrHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectLutr|UI")
	class ULutrHUDWidget* UIHUDWidget;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	bool bASCInputBound = false;
	
	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

private:
	UPROPERTY()
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLutr|UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
};
