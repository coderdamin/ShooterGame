// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletExplosionEffect.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABulletExplosionEffect::ABulletExplosionEffect()
	: ExplosionLightFadeOut(0.2f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ExplosionLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ExplosionLight"));
	RootComponent = ExplosionLight;
	ExplosionLight->AttenuationRadius = 400.0f;
	ExplosionLight->Intensity = 500.0f;
	ExplosionLight->LightColor = FColor(255, 185, 35);
	ExplosionLight->CastShadows = false;
	ExplosionLight->bVisible = true;
}

// Called when the game starts or when spawned
void ABulletExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float TimeAlive = GetWorld()->GetTimeSeconds() - CreationTime;
	const float TimeRemaining = FMath::Max(0.0f, ExplosionLightFadeOut - TimeAlive);
	if (TimeRemaining > 0.0f) {
		const float FadeAlpha = 1.0f - FMath::Square(TimeRemaining / ExplosionLightFadeOut);
		if (ExplosionLight != nullptr) {
			//ExplosionLight->SetBrightness(ExplosionLight->Intensity * FadeAlpha);
			ExplosionLight->SetIntensity(ExplosionLight->Intensity * FadeAlpha);
		}
	}
	else {
		Destroy();
	}
}

void ABulletExplosionEffect::PostInitializeComponents() {
	Super::PostInitializeComponents();
	if (ExplosionFX) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), GetActorRotation());
	}
	if (DefaultDecal.DecalMaterial) {
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
}
