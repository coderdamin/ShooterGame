// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerAIController.h"
#include "Character/KillerBot.h"
#include "Character/KillerCharacter.h"

AKillerAIController::AKillerAIController()
	: Enemy(nullptr)
	, LaseEnemyPos(ForceInitToZero)
{}

void AKillerAIController::SetEnemy(APawn* InPawn) {
	if (InPawn != nullptr) {
		Enemy = InPawn;
		SetLastEnemyPos(InPawn->GetActorLocation());
	}
	else {
		Enemy = nullptr;
		SetLastEnemyPos(FVector(ForceInitToZero));
	}
}

APawn* AKillerAIController::GetEnemy() {
	return Enemy;
}

bool AKillerAIController::ShootEnemy() {
	AKillerBot* Bot = Cast<AKillerBot>(GetPawn());
	if ((Bot == nullptr) || (!Bot->IsAlive())) {
		return false;
	}
	AKillerCharacter* EnemyCharacter = (Enemy != nullptr) ? Cast<AKillerCharacter>(Enemy) : nullptr;
	if ((EnemyCharacter != nullptr) && (EnemyCharacter->IsAlive())) {
		if (Bot->IsFiring()) {
			Bot->EndFiring();
		}
		else {
			Bot->BeginFiring();
		}
		return true;
	}
	return false;
}

void AKillerAIController::SetLastEnemyPos(FVector Pos) {
	LaseEnemyPos = Pos;
}

FVector AKillerAIController::GetLastEnemyPos() {
	return LaseEnemyPos;
}
