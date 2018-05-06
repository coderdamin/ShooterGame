// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KillerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API UKillerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	float GetMaxSpeed() const;
	
};
