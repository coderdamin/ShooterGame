// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Pickup.generated.h"

UCLASS()
class KILLERGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();
	virtual ~APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void BeginDestroy() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// Called by Server
	//virtual void DoPickup(class AKillerCharacter* pPawn) = 0;
	virtual void DoPickup(class AKillerCharacter* pPawn) {};

protected:
	void GiveGiftTo(class AKillerCharacter* pPawn);
	void DestroyWithCleanup();

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Pickup)
	UParticleSystemComponent* PickupPSC;
	
};
