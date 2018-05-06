// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KillerCharacter.h"
#include "KillerBot.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API AKillerBot : public AKillerCharacter
{
	GENERATED_UCLASS_BODY()
	//GENERATED_BODY()

public:
	AKillerBot();
	
	virtual bool IsFirstPerson() const { return false; };

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0.f);
};
