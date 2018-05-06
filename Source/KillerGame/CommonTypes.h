#pragma once

#include "KillerGame.h"
#include "CommonTypes.generated.h"

UENUM()
enum EMWeaponAmmoType {
	EMWeaponAmmoType_Instant,
	EMWeaponAmmoType_Projectile,
};

USTRUCT()
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditDefaultsOnly, Category = Decal)
		UMaterial* DecalMaterial;

	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float LifeSpan;

	FDecalData()
		: DecalSize(256.f)
		, LifeSpan(10.f)
	{}
};
