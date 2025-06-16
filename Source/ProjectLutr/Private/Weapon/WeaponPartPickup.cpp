#include "Weapon/WeaponPartPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Components/InventoryComponent.h"
#include "Weapon/WeaponPartData.h"
#include "GameFramework/Character.h"

AWeaponPartPickup::AWeaponPartPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponPartPickup::OnOverlapBegin);
}

void AWeaponPartPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponPartPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!WeaponPartData || !OtherActor) return;

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
		if (Inventory && Inventory->AddWeaponPart(WeaponPartData))
		{
			// Successfully picked up the part
			Destroy();
		}
	}
}