// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerPlayerController.h"
#include "Character/KillerCameraManager.h"

AKillerPlayerController::AKillerPlayerController(const FObjectInitializer&ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AKillerCameraManager::StaticClass();
}


