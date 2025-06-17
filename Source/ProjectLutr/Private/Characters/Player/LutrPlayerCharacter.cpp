// Copyright 2020 Dan Kestranek.


#include "Characters/Player/LutrPlayerCharacter.h"

#include "Characters/Components/InventoryComponent.h"
#include "Weapon/CraftingComponent.h"
#include "AI/LutrAIController.h"
#include "Camera/CameraComponent.h"
#include "Characters/Abilities/AttributeSets/LutrAttributeSetBase.h" // DONT REMOVE THIS IT WILL BREAK STUFF FOR SOME REASON
#include "Characters/Abilities/LutrAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "ProjectLutr/ProjectLutrGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/LutrPlayerController.h"
#include "Player/LutrPlayerState.h"
#include "UI/LutrFloatingStatusBarWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon/WeaponActor.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponInstance.h"

ALutrPlayerCharacter::ALutrPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComponent"));

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
	
	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());
	PlayerCameraComponent->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(PlayerCameraComponent);
	//FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(GetRootComponent());
	ThirdPersonMesh->SetOnlyOwnerSee(false);
	ThirdPersonMesh->SetOwnerNoSee(true);
	ThirdPersonMesh->bCastDynamicShadow = true;
	ThirdPersonMesh->CastShadow = true;

	AIControllerClass = ALutrAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void ALutrPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALutrPlayerCharacter, EquippedWeapon);
	UE_LOG(LogTemp, Warning, TEXT("Client [%s] received weapon: %s"),
	*GetName(),
	*GetNameSafe(EquippedWeapon));

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

		if (IsValid(PS->InventoryComponent))
		{
			if (StarterWeaponAsset)
			{
				UWeaponInstance* StarterWeapon = NewObject<UWeaponInstance>(this);
				StarterWeapon->WeaponData = StarterWeaponAsset;

				PS->InventoryComponent->AddWeapon(StarterWeapon);

				UE_LOG(LogTemp, Warning, TEXT("Server: Starter weapon added to %s inventory: %s"),
					*GetNameSafe(PS),
					*StarterWeaponAsset->GetName());
			}
		}
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
				//UIFloatingStatusBar->SetManaPercentage(GetMana() / GetMaxMana());
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

		if (!PS || !PS->InventoryComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryComponent not found on PlayerState"));
			return;
		}
	
		UWeaponDataAsset* StarterWeaponData = Cast<UWeaponDataAsset>(StaticLoadObject(
			UWeaponDataAsset::StaticClass(),
			nullptr,
			TEXT("/Game/ProjectLutr/Data/DA_AssaultRifle.DA_AssaultRifle")));
	
		if (StarterWeaponData)
		{
			UWeaponInstance* StarterWeapon = NewObject<UWeaponInstance>(this);
			StarterWeapon->WeaponData = StarterWeaponData;
	
			PS->InventoryComponent->AddWeapon(StarterWeapon);
	
			UE_LOG(LogTemp, Warning, TEXT("Starter weapon added to %s inventory: %s"),
				*GetNameSafe(PS),
				*StarterWeaponData->GetName());
		}
	}
}

void ALutrPlayerCharacter::OnRep_EquippedWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToComponent(
			ThirdPersonMesh,
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			FName("GunSocket"));

		UE_LOG(LogTemp, Warning, TEXT("Client: Re-attached EquippedWeapon on OnRep"));
	}
}
