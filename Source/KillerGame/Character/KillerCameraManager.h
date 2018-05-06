// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "KillerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API AKillerCameraManager : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()
	
protected:
	virtual void UpdateCamera(float DeltaTime) override;
	
private:
	float NormalFOV;
	float TargetingFOV;
};
