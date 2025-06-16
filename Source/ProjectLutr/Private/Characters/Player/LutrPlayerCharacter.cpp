// Copyright 2020 Dan Kestranek.


#include "Characters/Player/LutrPlayerCharacter.h"

#include "Characters/Components/InventoryComponent.h"
#include "Weapon/CraftingComponent.h"
#include "AI/LutrAIController.h"
#include "Characters/Abilities/AttributeSets/LutrAttributeSetBase.h" // DONT REMOVE THIS IT WILL BREAK STUFF FOR SOME REASON
#include "Characters/Abilities/LutrAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "ProjectLutr/ProjectLutrGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LutrPlayerController.h"
#include "Player/LutrPlayerState.h"
#include "UI/LutrFloatingStatusBarWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponInstance.h"

ALutrPlayerCharacter::ALutrPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComponent"));

	//GunComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); 

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIFloatingStatusBarComponent"));
	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));

	/*UIFloatingStatusBarClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/GASDocumentation/UI/UI_FloatingStatusBar_Hero.UI_FloatingStatusBar_Hero_C"));
	if (!UIFloatingStatusBarClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find UIFloatingStatusBarClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}*/

	AIControllerClass = ALutrAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

// Called to bind functionality to input
void ALutrPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	//BindASCInput();
}

// Server only
void ALutrPlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<ULutrAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		
		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		// End respawn specific things


		AddStartupEffects();

		AddCharacterAbilities();

		ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		InitializeFloatingStatusBar();
	}
}

ULutrFloatingStatusBarWidget * ALutrPlayerCharacter::GetFloatingStatusBar()
{
	return UIFloatingStatusBar;
}

//USkeletalMeshComponent * AGDHeroCharacter::GetGunComponent() const
//{
//	return GunComponent;
//}

void ALutrPlayerCharacter::FinishDying()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AProjectLutrGameMode* GM = Cast<AProjectLutrGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->HeroDied(GetController());
		}
	}

	Super::FinishDying();
}

/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void ALutrPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Only needed for Heroes placed in world and when the player is the Server.
	// On respawn, they are set up in PossessedBy.
	// When the player a client, the floating status bars are all set up in OnRep_PlayerState.
	InitializeFloatingStatusBar();
}

void ALutrPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if (GunComponent && GetMesh())
	//{
		//GunComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunSocket"));
	//}
}

void ALutrPlayerCharacter::InitializeFloatingStatusBar()
{
	// Only create once
	if (UIFloatingStatusBar || !AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Setup UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
	ALutrPlayerController* PC = Cast<ALutrPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->IsLocalPlayerController())
	{
		if (UIFloatingStatusBarClass)
		{
			UIFloatingStatusBar = CreateWidget<ULutrFloatingStatusBarWidget>(PC, UIFloatingStatusBarClass);
			if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
			{
				UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

				// Setup the floating status bar
				UIFloatingStatusBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
				UIFloatingStatusBar->SetManaPercentage(GetMana() / GetMaxMana());
			}
		}
	}
}

// Client only
void ALutrPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ALutrPlayerState* PS = GetPlayerState<ALutrPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<ULutrAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		//BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		ALutrPlayerController* PC = Cast<ALutrPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		// Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here
		InitializeFloatingStatusBar();


		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());
		
		UE_LOG(LogTemp, Error, TEXT("OnRepPlayerState"));

			if (UInventoryComponent* Inventory = PS->InventoryComponent)
			{
				UE_LOG(LogTemp, Error, TEXT("Inventory Component"));

				UWeaponInstance* FirstWeapon = Inventory->GetWeaponAt(0);
				if (FirstWeapon)
				{
					UE_LOG(LogTemp, Error, TEXT("Client Equipped: %s, Damage: %.1f"),
						*FirstWeapon->WeaponData->GetName(),
						FirstWeapon->GetDamage());
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("NO FIRST WEAPON"));
				}
			}
		
	}
}

// void ALutrPlayerCharacter::BindASCInput()
// {
// 	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
// 	{
// 		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/ProjectLutr"), FName("EGDAbilityInputID"));
// 		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
// 			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
//
// 		ASCInputBound = true;
// 	}
// }
