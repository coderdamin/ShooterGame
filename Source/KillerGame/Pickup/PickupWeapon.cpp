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
	if (Role != ROLE_Authority) {
		return false;
	}
	bool bRet = pPawn->PickWeapon(WeaponClass);
	return bRet;
}