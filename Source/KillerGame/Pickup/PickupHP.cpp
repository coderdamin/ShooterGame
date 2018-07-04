// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupHP.h"
#include "Character/KillerCharacter.h"

APickupHP::APickupHP()
{}

void APickupHP::BeginPlay() {
	Super::BeginPlay();
}

void APickupHP::DoPickup(class AKillerCharacter* pPawn) {
	if (pPawn == nullptr) {
		return;
	}
	//pPawn->AddHP();
	//DestroyWithCleanup();
}
