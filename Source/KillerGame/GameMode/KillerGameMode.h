// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KillerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API AKillerGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void FinishMatch();

private:
	class AKillerBot* SpawnBot(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnBots(int32 nCount);

public:
	TSubclassOf<APawn> BotPawnClass;
};
