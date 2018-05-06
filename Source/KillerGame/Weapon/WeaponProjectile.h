// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "WeaponProjectile.generated.h"

USTRUCT()
struct FProjectileBulletData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ProjectileLife;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ExplosionRadius;

	FProjectileBulletData()
		: ProjectileClass(nullptr)
		, ProjectileLife(10.0f)
		, ExplosionDamage(100.0f)
		, ExplosionRadius(300.0f)
	{}
};

/**
 * 
 */
UCLASS()
class KILLERGAME_API AWeaponProjectile : public AWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeaponProjectile();
	
	FProjectileBulletData& GetProjectileConfig() {
		return ProjectileConfig;
	}

protected:
	virtual void OnFiring() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	FProjectileBulletData ProjectileConfig;
	
};
