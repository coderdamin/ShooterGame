// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "WeaponInstant.generated.h"


USTRUCT()
struct FWeaponInstantConfig {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fFiringSpreadInit;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fFiringSpreadMax;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fFiringSpreadIncrement;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fTargetingSpreadMod;

	FWeaponInstantConfig()
		: fFiringSpreadMax(5.0f)
		, fFiringSpreadIncrement(1.0f)
		, fTargetingSpreadMod(0.25)
	{}
};

/**
 * 
 */
UCLASS()
class KILLERGAME_API AWeaponInstant : public AWeapon
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWeaponInstant();
	
private:
	float GetCurrentSpread() const;
	void RefreshCurrentSpread(bool bForceInit = false);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProperty) const override;

protected:
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace);

	virtual void OnFiring() override;

	void ProcessInstantHit(const FHitResult&Impact, const FVector&Origin, const FVector&ShootDir, int32 RandomSeed, float Reticlespread);
	void SpawnTrailEffect(const FVector&EndPoint);
	void SpawnImpactEffects(const FHitResult&Impact);

public:
	virtual bool EndFiring();

public:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FWeaponInstantConfig m_InstantConfig;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	class UParticleSystem* m_pTrailFX;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName m_TrailTargetParam;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class ABulletImpactEffect> m_BulletImpactEffectClass;

private:
	float m_fCurrentSpread;
};
