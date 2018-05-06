// Fill out your copyright notice in the Description page of Project Settings.
#include "KillerBot.h"
#include "KillerGame.h"
#include "AI/KillerAIController.h"

AKillerBot::AKillerBot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<AAIController> AIControllerOb(TEXT("Blueprint'/Game/Player/BP_KillerAIController.BP_KillerAIController'"));
	//AIControllerClass = AIControllerOb.Class;
}

void AKillerBot::FaceRotation(FRotator NewControlRotation, float DeltaTime) {
	FRotator CurrentRotator = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.0f);
	Super::FaceRotation(CurrentRotator, DeltaTime);
}
