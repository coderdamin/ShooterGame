// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupWeapon.h"
#include "Character/KillerCharacter.h"
#include "Weapon/Weapon.h"

APickupWeapon::APickupWeapon()
{}

void APickupWeapon::BeginPlay() {
	Super::BeginPlay();
}

void APickupWeapon::DoPickup(class AKillerCharacter* pPawn) {
	if (pPawn != nullptr) {
		pPawn->PickWeapon(WeaponClass);
		DestroyWithCleanup();
	}
}
