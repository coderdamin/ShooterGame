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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual bool GiveGiftTo(class AKillerCharacter* pPawn);

private:
	void DoPickup(class AKillerCharacter* pPawn);

	UFUNCTION(reliable, server, WithValidation)
	void ServerDoPickup(class AKillerCharacter* pPawn);

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Pickup)
	UParticleSystemComponent* PickupPSC;
	
};