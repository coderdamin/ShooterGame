// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerPlayerController.h"
#include "Character/KillerCharacter.h"
#include "Character/KillerCameraManager.h"

AKillerPlayerController::AKillerPlayerController(const FObjectInitializer&ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AKillerCameraManager::StaticClass();
}

void AKillerPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AKillerPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AKillerPlayerController::AddYawInput);
	InputComponent->BindAxis("Turn", this, &AKillerPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AKillerPlayerController::AddPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AKillerPlayerController::OnJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AKillerPlayerController::OnStopJump);
	InputComponent->BindAction("EnterRunState", IE_Pressed, this, &AKillerPlayerController::OnEnterRunningState);
	InputComponent->BindAction("EnterRunState", IE_Released, this, &AKillerPlayerController::OnOutRunningState);
	InputComponent->BindAction("EnterAimState", IE_Pressed, this, &AKillerPlayerController::OnEnterAimingState);
	InputComponent->BindAction("EnterAimState", IE_Released, this, &AKillerPlayerController::OnOutAimingState);
	InputComponent->BindAction("Firing", IE_Pressed, this, &AKillerPlayerController::OnBeginFiring);
	InputComponent->BindAction("Firing", IE_Released, this, &AKillerPlayerController::OnEndFiring);
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AKillerPlayerController::OnNextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AKillerPlayerController::OnPrevWeapon);
	InputComponent->BindAction("ReloadAmmo", IE_Pressed, this, &AKillerPlayerController::OnReloadAmmo);
	InputComponent->BindAction("SetBursting", IE_Pressed, this, &AKillerPlayerController::OnSetBursting);
}

//void AKillerPlayerController::MoveForward(float fVal) {
//	this->AddPitchInput(fVal);
//}

void AKillerPlayerController::OnMoveForward(float fVal) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->MoveForward(fVal);
	}
}

void AKillerPlayerController::OnMoveRight(float fVal) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->MoveRight(fVal);
	}
}

void AKillerPlayerController::OnTurn(float fVal) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->Turn(fVal);
	}
}

void AKillerPlayerController::OnLookUp(float fVal) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->LookUp(fVal);
	}
}

void AKillerPlayerController::OnJump() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->Jump();
	}
}

void AKillerPlayerController::OnStopJump() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->StopJump();
	}
}

void AKillerPlayerController::OnEnterRunningState() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->EnterRunningState();
	}
}

void AKillerPlayerController::OnOutRunningState() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->OutRunningState();
	}
}

void AKillerPlayerController::OnEnterAimingState() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->EnterAimingState();
	}
}

void AKillerPlayerController::OnOutAimingState() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->OutAimingState();
	}
}

void AKillerPlayerController::OnBeginFiring() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->BeginFiring();
	}
}

void AKillerPlayerController::OnEndFiring() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->EndFiring();
	}
}

void AKillerPlayerController::OnNextWeapon() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->NextWeapon();
	}
}

void AKillerPlayerController::OnPrevWeapon() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->PrevWeapon();
	}
}

void AKillerPlayerController::OnReloadAmmo() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->ReloadAmmo();
	}
}

void AKillerPlayerController::OnSetBursting() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->SetBursting();
	}
}

void AKillerPlayerController::OnOpenScoreBoard() {
}

void AKillerPlayerController::OnOpenChatInput() {
}

AKillerCharacter* AKillerPlayerController::GetKillerCharacter() {
	APawn* pPawn = GetPawn();
	return (pPawn != nullptr) ? Cast<AKillerCharacter>(pPawn) : nullptr;
}