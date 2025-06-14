#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPartPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWeaponPartData;

UCLASS()
class PROJECTLUTR_API AWeaponPartPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponPartPickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

public:	
	// Called when another actor overlaps with this pickup
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectLutr|Pickup")
	UWeaponPartData* WeaponPartData;
};