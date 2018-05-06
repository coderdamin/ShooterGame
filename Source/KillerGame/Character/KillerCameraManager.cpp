// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerCameraManager.h"
#include "KillerCharacter.h"
#include "KillerGame.h"

AKillerCameraManager::AKillerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, NormalFOV(90.0f)
	, TargetingFOV(60.0f)
{
	ViewPitchMax = 87.0f;
	ViewPitchMin = -87.0f;
	bAlwaysApplyModifiers = true;
}

void AKillerCameraManager::UpdateCamera(float DeltaTime) {
	AKillerCharacter* pPawn = PCOwner ? Cast<AKillerCharacter>(PCOwner->GetPawn()) : nullptr;
	if (pPawn != nullptr && pPawn->IsFirstPerson()) {
		const float TargetFov = pPawn->IsAimingState() ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFov, DeltaTime, 20.0f);
	}
	Super::UpdateCamera(DeltaTime);

	// 第一人称Look Up处理
	if (pPawn != nullptr && pPawn->IsFirstPerson()) {
		pPawn->OnCameraUpdate(GetCameraLocation(), GetCameraRotation());
	}
}

