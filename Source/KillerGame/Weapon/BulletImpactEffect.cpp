// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletImpactEffect.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABulletImpactEffect::ABulletImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;
	bAutoDestroyWhenFinished = true;
}

// Called when the game starts or when spawned
void ABulletImpactEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletImpactEffect::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if (DefaultFX != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(this, DefaultFX, GetActorLocation(), GetActorRotation());
	}
	if (DefaultDecal.DecalMaterial != nullptr) {
		FRotator DecalRotation = m_HitInfo.ImpactNormal.Rotation();
		DecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial
			, FVector(DefaultDecal.DecalSize, DefaultDecal.DecalSize, 1.0f)
			, m_HitInfo.Component.Get()
			, m_HitInfo.BoneName
			, m_HitInfo.ImpactPoint
			, DecalRotation
			, EAttachLocation::KeepWorldPosition
			, DefaultDecal.LifeSpan);
	}
	//SetLifeSpan(DefaultDecal.LifeSpan);
}
