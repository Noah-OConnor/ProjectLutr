// Copyright 2025 Project Lutr


#include "Weapon/WeaponActor.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponInstance.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponActor, ReplicatedWeaponData);
}

void AWeaponActor::SetWeaponInstance(UWeaponInstance* WeaponInstance)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] SetWeaponInstance called. Has StaticMesh: %d, Has SkeletalMesh: %d"),
		*GetName(),
		WeaponInstance && WeaponInstance->GetStaticMesh() != nullptr,
		WeaponInstance && WeaponInstance->GetSkeletalMesh() != nullptr);
	
	ReplicatedWeaponData = WeaponInstance ? WeaponInstance->WeaponData : nullptr;

	if (ReplicatedWeaponData)
	{
		ApplyWeaponData(ReplicatedWeaponData);
	}
	
	if (WeaponInstance)
	{
		if (WeaponInstance->GetStaticMesh())
		{
			StaticMesh->SetStaticMesh(WeaponInstance->GetStaticMesh());
		}
		else if (WeaponInstance->GetSkeletalMesh())
		{
			SkeletalMesh->SetSkeletalMesh(WeaponInstance->GetSkeletalMesh());
		}
	}
}

void AWeaponActor::OnRep_WeaponData()
{
	ApplyWeaponData(ReplicatedWeaponData);
}

void AWeaponActor::ApplyWeaponData(UWeaponDataAsset* Data)
{
	if (!Data)
		return;

	if (Data->StaticMesh)
	{
		StaticMesh->SetStaticMesh(Data->StaticMesh);
		StaticMesh->SetVisibility(true);
		SkeletalMesh->SetVisibility(false);
	}
	else if (Data->SkeletalMesh)
	{
		SkeletalMesh->SetSkeletalMesh(Data->SkeletalMesh);
		SkeletalMesh->SetVisibility(true);
		StaticMesh->SetVisibility(false);
	}
}