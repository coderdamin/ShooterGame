// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponProjectile.h"
#include "kismet/GameplayStatics.h"
#include "Projectile.h"

AWeaponProjectile::AWeaponProjectile() 
{}

void AWeaponProjectile::OnFiring() {
	FVector ShootDir = GetAdjustedAim();
	FVector Origin = GetMuzzleLocation();

	FTransform SpawnTrans(ShootDir.Rotation(), Origin);
	//BeginDeferredActorSpawnFromClass
	AProjectile* Projectile = Cast<AProjectile>(UGameplayStatics::BeginSpawningActorFromClass(this
		, ProjectileConfig.ProjectileClass
		, SpawnTrans));
	if (Projectile)
	{
		Projectile->Instigator = Instigator;
		Projectile->SetOwner(this);
		//Projectile->InitVelocity(ShootDir);

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTrans);
	}
}
