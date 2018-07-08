// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponProjectile.h"
#include "Character/KillerCharacter.h"
#include "Player/KillerPlayerController.h"
#include "Projectile.h"

AWeaponProjectile::AWeaponProjectile() 
{}

void AWeaponProjectile::OnFiring() {
	FVector ShootDir = GetAdjustedAim();
	FVector Origin = GetMuzzleLocation();

	AKillerPlayerController* pc = nullptr;
	if (m_pOwner) {
		pc = Cast<AKillerPlayerController>(m_pOwner->GetController());
	}
	if (pc) {
		pc->Server_LaunchProjectile(Origin, ShootDir);
	}
}
