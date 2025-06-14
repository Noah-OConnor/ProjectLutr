// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LutrCharacterBase.h"
#include "LutrPlayerCharacter.generated.h"

class UInventoryComponent;
/**
 * A player or AI controlled hero character.
 */
UCLASS()
class PROJECTLUTR_API ALutrPlayerCharacter : public ALutrCharacterBase
{
	GENERATED_BODY()

public:
	ALutrPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	//class USpringArmComponent* GetCameraBoom();

	//class UCameraComponent* GetFollowCamera();

	//UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Camera")
	//float GetStartingCameraBoomArmLength();

	//UFUNCTION(BlueprintCallable, Category = "ProjectLutr|Camera")
	//FVector GetStartingCameraBoomLocation();

	class ULutrFloatingStatusBarWidget* GetFloatingStatusBar();

	//USkeletalMeshComponent* GetGunComponent() const;

	virtual void FinishDying() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectLutr|Inventory")
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|Camera")
	float BaseLookUpRate = 45.0f;

	//UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Camera")
	//float StartingCameraBoomArmLength;

	//UPROPERTY(BlueprintReadOnly, Category = "ProjectLutr|Camera")
	//FVector StartingCameraBoomLocation;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProjectLutr|Camera")
	//class USpringArmComponent* CameraBoom;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProjectLutr|Camera")
	//class UCameraComponent* FollowCamera;

	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	//USkeletalMeshComponent* GunComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectLutr|UI")
	TSubclassOf<class ULutrFloatingStatusBarWidget> UIFloatingStatusBarClass;

	UPROPERTY()
	class ULutrFloatingStatusBarWidget* UIFloatingStatusBar;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "ProjectLutr|UI")
	class UWidgetComponent* UIFloatingStatusBarComponent;

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Mouse
	void LookUp(float Value);

	// Gamepad
	void LookUpRate(float Value);

	// Mouse
	void Turn(float Value);

	// Gamepad
	void TurnRate(float Value);

	// Mouse + Gamepad
	void MoveForward(float Value);

	// Mouse + Gamepad
	void MoveRight(float Value);

	// Creates and initializes the floating status bar for heroes.
	// Safe to call many times because it checks to make sure it only executes once.
	UFUNCTION()
	void InitializeFloatingStatusBar();

	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
};
