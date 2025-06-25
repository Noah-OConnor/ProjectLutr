// Copyright 2020 Dan Kestranek.

#include "ProjectLutr/ProjectLutrGameMode.h"
#include "Engine/World.h"
#include "Characters/Player/LutrPlayerCharacter.h"
#include "Player/LutrPlayerController.h"
#include "Player/LutrPlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AProjectLutrGameMode::AProjectLutrGameMode()
{
	RespawnDelay = 5.0f;

	/*HeroClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/GASDocumentation/Characters/Hero/BP_HeroCharacter.BP_HeroCharacter_C"));
	if (!HeroClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Failed to find HeroClass. If it was moved, please update the reference location in C++."), *FString(__FUNCTION__));
	}*/
}

void AProjectLutrGameMode::HeroDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);

	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &AProjectLutrGameMode::RespawnHero, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);

	ALutrPlayerController* PC = Cast<ALutrPlayerController>(Controller);
	if (PC)
	{
		PC->SetRespawnCountdown(RespawnDelay);
	}
}

void AProjectLutrGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get the enemy hero spawn point
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor->GetName() == FString("EnemyHeroSpawn"))
		{
			EnemySpawnPoint = Actor;
			break;
		}
	}
}

void AProjectLutrGameMode::RespawnHero(AController * Controller)
{
	if (Controller->IsPlayerController())
	{
		// Respawn player hero
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ALutrPlayerCharacter* Hero = GetWorld()->SpawnActor<ALutrPlayerCharacter>(HeroClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
	else
	{
		// Respawn AI hero
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ALutrPlayerCharacter* Hero = GetWorld()->SpawnActor<ALutrPlayerCharacter>(HeroClass, EnemySpawnPoint->GetActorTransform(), SpawnParameters);
		
		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
}
