// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectLutrGameMode.generated.h"

UCLASS(minimalapi)
class AProjectLutrGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectLutrGameMode();

	void HeroDied(AController* Controller);

protected:
	float RespawnDelay;

	TSubclassOf<class ALutrPlayerCharacter> HeroClass;

	AActor* EnemySpawnPoint;

	virtual void BeginPlay() override;

	void RespawnHero(AController* Controller);
};
