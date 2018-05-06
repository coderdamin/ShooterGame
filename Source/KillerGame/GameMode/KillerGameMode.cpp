// Fill out your copyright notice in the Description page of Project Settings.
#include "KillerGameMode.h"
#include "KillerGame.h"
#include "Player/KillerPlayerController.h"
#include "Character/KillerBot.h"

AKillerGameMode::AKillerGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Blueprint'/Game/Character/BP_KillerCharacter.BP_KillerCharacter'"));
	if (PlayerPawnObject.Object) {
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}
	PlayerControllerClass = AKillerPlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> BotPawnObject(TEXT("Blueprint'/Game/Character/BP_KillerBot.BP_KillerBot'"));
	if (BotPawnObject.Object) {
		BotPawnClass = (UClass*)BotPawnObject.Object->GeneratedClass;
	}
}

void AKillerGameMode::HandleMatchHasStarted() {
	Super::HandleMatchHasStarted();
	SpawnBots(10);
}

void AKillerGameMode::HandleMatchHasEnded() {
	Super::HandleMatchHasEnded();
}

void AKillerGameMode::FinishMatch() {
	if (IsMatchInProgress()) {
		EndMatch();
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It) {
			(*It)->GameHasEnded(nullptr);
		}
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It) {
			(*It)->TurnOff();
		}
	}
}

AKillerBot* AKillerGameMode::SpawnBot(FVector SpawnLocation, FRotator SpawnRotation) {
	if (BotPawnClass != nullptr) {
		FActorSpawnParameters SpawnInfo;
		AKillerBot* pBot = GetWorld()->SpawnActor<AKillerBot>(BotPawnClass, SpawnLocation, SpawnRotation, SpawnInfo);
		if (pBot) {
			pBot->SpawnDefaultController();
			return pBot;
		}
	}
	return nullptr;
}

void AKillerGameMode::SpawnBots(int32 nCount) {
	//for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	FString Portal("BotStart");
	AActor* StartSpot = nullptr;
	AKillerBot* pBot = nullptr;
	for (int i = 0; i < nCount; ++i) {
		pBot = SpawnBot(FVector(ForceInitToZero), FRotator(ForceInitToZero));
		if (pBot) {
			StartSpot = FindPlayerStart(pBot->GetController(), Portal);
			if (StartSpot) {
				FVector ActorLocation = StartSpot->GetActorLocation();
				const FRotator ActorRotation = StartSpot->GetActorRotation();
				//pBot->GetController()->SetInitialLocationAndRotation(ActorLocation, ActorRotation);
				//bool AActor::TeleportTo(const FVector& DestLocation, const FRotator& DestRotation, bool bIsATest, bool bNoCheck)
				pBot->TeleportTo(ActorLocation, ActorRotation, false, true);
			}
		}
	}
}
