// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "PickupHP.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API APickupHP : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickupHP();

	virtual void DoPickup(class AKillerCharacter* pPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
