// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "WeaponProjectile.h"
#include "Projectile.generated.h"

UCLASS()
class KILLERGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitVelocity(FVector&ShootDir);

	void DoExplode(const FHitResult& HitResult);

	void DisableAndDestroy(float DelayTime);

	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);
	
public:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UParticleSystemComponent* ParticleComp;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class ABulletExplosionEffect> m_BulletExplosionClass;

	struct FProjectileBulletData ProjectileConfig;

	bool bExploded;
};
