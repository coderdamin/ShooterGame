// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KillerAIController.generated.h"

/**
 * 
 */
UCLASS()
class KILLERGAME_API AKillerAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKillerAIController();
	
	UFUNCTION(BlueprintCallable, Category = Enemy)
	void SetEnemy(APawn* InPawn);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	APawn* GetEnemy();

	UFUNCTION(BlueprintCallable, Category = Enemy)
	bool ShootEnemy();

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void SetLastEnemyPos(FVector Pos);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	FVector GetLastEnemyPos();

public:
	APawn * Enemy;
	FVector LaseEnemyPos;
};
