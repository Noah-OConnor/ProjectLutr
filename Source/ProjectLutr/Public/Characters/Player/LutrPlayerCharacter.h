// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LutrCharacterBase.h"
#include "LutrPlayerCharacter.generated.h"

//class UInventoryComponent;
class UCraftingComponent;
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

	class ULutrFloatingStatusBarWidget* GetFloatingStatusBar();

	//USkeletalMeshComponent* GetGunComponent() const;

	virtual void FinishDying() override;

protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectLutr|Inventory")
	//UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectLutr|Inventory")
	UCraftingComponent* CraftingComponent;

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

	// Creates and initializes the floating status bar for heroes.
	// Safe to call many times because it checks to make sure it only executes once.
	UFUNCTION()
	void InitializeFloatingStatusBar();

	// Client only
	virtual void OnRep_PlayerState() override;
};
