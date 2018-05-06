// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonTypes.h"
#include "BulletExplosionEffect.generated.h"

UCLASS()
class KILLERGAME_API ABulletExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletExplosionEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	class UParticleSystem* ExplosionFX;
	
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	struct FDecalData DefaultDecal;

	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	UPointLightComponent* ExplosionLight;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	float ExplosionLightFadeOut;

	FHitResult m_HitInfo;
};
