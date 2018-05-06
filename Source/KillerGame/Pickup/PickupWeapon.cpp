// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupWeapon.h"
#include "Character/KillerCharacter.h"
#include "Weapon/Weapon.h"

APickupWeapon::APickupWeapon()
{}

void APickupWeapon::BeginPlay() {
	Super::BeginPlay();
}

bool APickupWeapon::GiveGiftTo(class AKillerCharacter* pPawn) {
	if (pPawn == nullptr) {
		return false;
	}
	return pPawn->PickWeapon(WeaponClass);
}