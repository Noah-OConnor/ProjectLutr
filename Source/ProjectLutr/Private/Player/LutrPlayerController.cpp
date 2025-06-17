// Copyright 2020 Dan Kestranek.


#include "Player/LutrPlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/Components/LutrCharacterMovementComponent.h"
#include "Characters/Components/InventoryComponent.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Characters/Player/LutrPlayerCharacter.h"
#include "Player/LutrPlayerState.h"
#include "UI/LutrDamageTextWidgetComponent.h"
#include "UI/LutrHUDWidget.h"
#include "Weapon/WeaponPartData.h"

void ALutrPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Register mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}
}

void ALutrPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALutrPlayerController::HandleMove);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALutrPlayerController::HandleLook);
		
		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ALutrPlayerController::StartSprinting);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALutrPlayerController::StopSprinting);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ALutrPlayerController::HandleJumpPressed);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ALutrPlayerController::HandleJumpReleased);

		Input->BindAction(EquipWeaponAction, ETriggerEvent::Started, this, &ALutrPlayerController::HandleEquipWeaponPressed);

		Input->BindAction(ADSAction, ETriggerEvent::Started, this, &ALutrPlayerController::StartADS);
		Input->BindAction(ADSAction, ETriggerEvent::Completed, this, &ALutrPlayerController::StopADS);
		
		//Input->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ALutrPlayerController::HandleOpenInventory);
	}
	BindASCInput();
}

void ALutrPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<ULutrHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	UIHUDWidget->SetCurrentCapacityReserve(PS->GetCapacityReserve());
	UIHUDWidget->SetMaxCapacityReserve(PS->GetMaxCapacityReserve());
	UIHUDWidget->SetCurrentMana(PS->GetMana());
	UIHUDWidget->SetMaxMana(PS->GetMaxMana());
	UIHUDWidget->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
	UIHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
	UIHUDWidget->SetManaRegenRate(PS->GetManaRegenRate());
	UIHUDWidget->SetCurrentStamina(PS->GetStamina());
	UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
	UIHUDWidget->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	UIHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
	UIHUDWidget->SetExperience(PS->GetXP());
	UIHUDWidget->SetGold(PS->GetGold());
	UIHUDWidget->SetHeroLevel(PS->GetCharacterLevel());
}

ULutrHUDWidget * ALutrPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void ALutrPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ALutrCharacterBase* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		ULutrDamageTextWidgetComponent* DamageText = NewObject<ULutrDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool ALutrPlayerController::ShowDamageNumber_Validate(float DamageAmount, ALutrCharacterBase* TargetCharacter)
{
	return true;
}

void ALutrPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
	}
}

bool ALutrPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

void ALutrPlayerController::HandleMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		FRotator ControlRot = GetControlRotation();
		FRotator YawRotation(0.f, ControlRot.Yaw, 0.f);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALutrPlayerController::HandleLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}


void ALutrPlayerController::HandleOpenInventory(const FInputActionValue& ActionValue)
{
	if (!InventoryWidget && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryWidgetClass);
	}

	if (InventoryWidget && !InventoryWidget->IsInViewport())
	{
		// 🔹 Get your character
		ALutrPlayerCharacter* PlayerChar = Cast<ALutrPlayerCharacter>(GetPawn());
		if (!PlayerChar) return;

		// 🔹 Get InventoryComponent
		UInventoryComponent* InventoryComp = PlayerChar->FindComponentByClass<UInventoryComponent>();
		UE_LOG(LogTemp, Warning, TEXT("Inventory count: %d"), InventoryComp->GetInventory().Num());

		if (!InventoryComp) return;

		// 🔹 Filter for base parts
		TArray<UWeaponPartData*> BaseParts;
		for (UWeaponPartData* Part : InventoryComp->GetInventory())
		{
			if (Part && Part->PartType == EWeaponPartType::Base)
			{
				BaseParts.Add(Part);
			}
		}

		// 🔹 Pass to inventory UI
		InventoryWidget->PopulateBaseParts(BaseParts);

		InventoryWidget->AddToViewport();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void ALutrPlayerController::StartSprinting()
{
	if (ALutrPlayerCharacter* LutrCharacter = Cast<ALutrPlayerCharacter>(GetPawn()))
	{
		if (ULutrCharacterMovementComponent* MoveComp = Cast<ULutrCharacterMovementComponent>(LutrCharacter->GetCharacterMovement()))
		{
			MoveComp->StartSprinting();
		}
	}
}

void ALutrPlayerController::StopSprinting()
{
	if (ALutrPlayerCharacter* LutrCharacter = Cast<ALutrPlayerCharacter>(GetPawn()))
	{
		if (ULutrCharacterMovementComponent* MoveComp = Cast<ULutrCharacterMovementComponent>(LutrCharacter->GetCharacterMovement()))
		{
			MoveComp->StopSprinting();
		}
	}
}

void ALutrPlayerController::HandleJumpPressed()
{
	if (ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Jump));
		}
	}
}

void ALutrPlayerController::HandleJumpReleased()
{
	if (ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			ASC->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::Jump));
		}
	}
}


void ALutrPlayerController::StartADS()
{
	if (ALutrPlayerCharacter* LutrCharacter = Cast<ALutrPlayerCharacter>(GetPawn()))
	{
		if (ULutrCharacterMovementComponent* MoveComp = Cast<ULutrCharacterMovementComponent>(LutrCharacter->GetCharacterMovement()))
		{
			MoveComp->StartAimDownSights();
		}
	}
}

void ALutrPlayerController::StopADS()
{
	if (ALutrPlayerCharacter* LutrCharacter = Cast<ALutrPlayerCharacter>(GetPawn()))
	{
		if (ULutrCharacterMovementComponent* MoveComp = Cast<ULutrCharacterMovementComponent>(LutrCharacter->GetCharacterMovement()))
		{
			MoveComp->StopAimDownSights();
		}
	}
}

void ALutrPlayerController::HandleEquipWeaponPressed()
{
	if (ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::EquipWeapon));
		}
	}
}


// Server only
void ALutrPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ALutrPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();

	BindASCInput();
}

void ALutrPlayerController::BindASCInput()
{
	if (bASCInputBound)
		return;

	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (!PS)
		return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	if (!ASC || !InputComponent)
		return;

	FTopLevelAssetPath EnumPath = FTopLevelAssetPath(TEXT("/Script/ProjectLutr"), TEXT("EAbilityInputID"));

	ASC->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(
		TEXT("ConfirmTarget"), TEXT("CancelTarget"),
		EnumPath,
		static_cast<int32>(EAbilityInputID::Confirm),
		static_cast<int32>(EAbilityInputID::Cancel)
	));

	bASCInputBound = true;
}