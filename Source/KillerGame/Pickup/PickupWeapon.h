// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API APickupWeapon : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual bool GiveGiftTo(class AKillerCharacter* pPawn);
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;
	
};
