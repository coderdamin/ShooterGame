// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "CommonTypes.h"
#include "PickupAmmo.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API APickupAmmo : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickupAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual bool GiveGiftTo(class AKillerCharacter* pPawn);
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 AmmoCount;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	TEnumAsByte<EMWeaponAmmoType> AmmoType;
};
