// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerMovementComponent.h"
#include "KillerCharacter.h"


float UKillerMovementComponent::GetMaxSpeed() const {
	float fMaxSpeed = Super::GetMaxSpeed();
	const AKillerCharacter* pPawn = Cast<AKillerCharacter>(PawnOwner);
	if (pPawn) {
		if (pPawn->IsAimingState()) {
			fMaxSpeed *= pPawn->GetTargetingSpeedModifier();
		}
		else if (pPawn->IsRuningState()) {
			fMaxSpeed *= pPawn->GetRunningSpeedModifier();
		}
	}
	return fMaxSpeed;
}

