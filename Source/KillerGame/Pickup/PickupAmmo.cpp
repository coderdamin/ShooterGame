// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupAmmo.h"
#include "Character/KillerCharacter.h"
#include "Weapon/Weapon.h"

APickupAmmo::APickupAmmo()
{}

void APickupAmmo::BeginPlay() {
	Super::BeginPlay();
}

void APickupAmmo::DoPickup(class AKillerCharacter* pPawn) {	
	if (pPawn == nullptr) {
		return;
	}
	TArray<AWeapon *> WeaponAll = pPawn->GetWeaponList();
	for (auto weapon : WeaponAll) {
		if (weapon && weapon->CheckAmmoType(AmmoType)) {
			weapon->AddAmmo(AmmoCount);
			DestroyWithCleanup();
			break;
		}
	}
}


