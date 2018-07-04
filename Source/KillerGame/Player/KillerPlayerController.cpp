// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerPlayerController.h"
#include "Character/KillerCharacter.h"
#include "Character/KillerCameraManager.h"
#include "Pickup/Pickup.h"

DEFINE_LOG_CATEGORY(LOG_CATEGORY_NAME)

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
		pCharacter->AddControllerYawInput(fVal);
	}
}

void AKillerPlayerController::OnLookUp(float fVal) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->AddControllerPitchInput(fVal);
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
	if (Role != ROLE_Authority) {
		Server_OnEnterRunningState(true);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->EnterRunningState();
		}
	}
}

void AKillerPlayerController::OnOutRunningState() {
	if (Role != ROLE_Authority) {
		Server_OnEnterRunningState(false);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->OutRunningState();
		}
	}
}

void AKillerPlayerController::OnEnterAimingState() {
	if (Role != ROLE_Authority) {
		Server_OnEnterAimingState(true);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->EnterAimingState();
		}
	}
}

void AKillerPlayerController::OnOutAimingState() {
	if (Role != ROLE_Authority) {
		Server_OnEnterAimingState(true);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->OutAimingState();
		}
	}
}

void AKillerPlayerController::OnBeginFiring() {
	if (Role != ROLE_Authority) {
		Server_OnBeginFiring(true);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->BeginFiring();
		}
	}
}

void AKillerPlayerController::OnEndFiring() {
	if (Role != ROLE_Authority) {
		Server_OnBeginFiring(false);
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->EndFiring();
		}
	}
}

void AKillerPlayerController::OnNextWeapon() {
	if (Role != ROLE_Authority) {
		Server_OnNextWeapon(true);
	}
	//if ((Role == ROLE_Authority) || (IsLocalController())) {
	if (Role == ROLE_Authority) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->NextWeapon();
		}
	}
}

void AKillerPlayerController::OnPrevWeapon() {
	if (Role != ROLE_Authority) {
		Server_OnNextWeapon(false);
	}
	//if ((Role == ROLE_Authority) || (IsLocalController())) {
	if (Role == ROLE_Authority) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->PrevWeapon();
		}
	}
}

void AKillerPlayerController::OnReloadAmmo() {
	if (Role != ROLE_Authority) {
		Server_OnReloadAmmo();
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->ReloadAmmo();
		}
	}
}

void AKillerPlayerController::OnSetBursting() {
	if (Role != ROLE_Authority) {
		Server_OnSetBursting();
	}
	if ((Role == ROLE_Authority) || (IsLocalController())) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter != nullptr) {
			pCharacter->SetBursting();
		}
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

bool AKillerPlayerController::Server_OnEnterRunningState_Validate(bool bEnter) {
	return true;
}
void AKillerPlayerController::Server_OnEnterRunningState_Implementation(bool bEnter) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Set RunningState None Character!"));
		return;
	}
	if (bEnter) {
		pCharacter->EnterRunningState();
	}
	else {
		pCharacter->OutRunningState();
	}
}

bool AKillerPlayerController::Server_OnEnterAimingState_Validate(bool bEnter) {
	return true;
}
void AKillerPlayerController::Server_OnEnterAimingState_Implementation(bool bEnter) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Set AimingState None Character!"));
		return;
	}
	if (bEnter) {
		pCharacter->EnterAimingState();
	}
	else {
		pCharacter->OutAimingState();
	}
}

bool AKillerPlayerController::Server_OnBeginFiring_Validate(bool bBegin) {
	return true;
}
void AKillerPlayerController::Server_OnBeginFiring_Implementation(bool bBegin) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Begin Firing None Character!"));
		return;
	}
	if (bBegin) {
		pCharacter->BeginFiring();
	}
	else {
		pCharacter->EndFiring();
	}
}

bool AKillerPlayerController::Server_OnNextWeapon_Validate(bool bNext) {
	return true;
}
void AKillerPlayerController::Server_OnNextWeapon_Implementation(bool bNext) {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Next Weapon None Character!"));
		return;
	}
	if (bNext) {
		pCharacter->NextWeapon();
	}
	else {
		pCharacter->PrevWeapon();
	}
}

bool AKillerPlayerController::Server_OnSetBursting_Validate() {
	return true;
}
void AKillerPlayerController::Server_OnSetBursting_Implementation() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Set Bursting None Character!"));
		return;
	}
	pCharacter->SetBursting();
}

bool AKillerPlayerController::Server_OnReloadAmmo_Validate() {
	return true;
}
void AKillerPlayerController::Server_OnReloadAmmo_Implementation() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		pCharacter->ReloadAmmo();
	}
}

void AKillerPlayerController::PickupGift(class APickup* pPickup) {
	if (pPickup == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Pick Gift None Pickup!"));
		return;
	}
	if (Role == ROLE_Authority) {
		AKillerCharacter* pCharacter = GetKillerCharacter();
		if (pCharacter == nullptr) {
			UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Pick Weapon None Character!"));
			return;
		}
		pPickup->DoPickup(pCharacter);
	}
	else {
		Server_OnPickupGift(pPickup);
	}
}

bool AKillerPlayerController::Server_OnTakeDamage_Validate() {
	return true;
}
void AKillerPlayerController::Server_OnTakeDamage_Implementation() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		//pCharacter->TakeDamage();
	}
}

bool AKillerPlayerController::Server_OnPickupGift_Validate(class APickup* pPickup) {
	return true;
}
void AKillerPlayerController::Server_OnPickupGift_Implementation(class APickup* pPickup) {
	if (pPickup == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Pick Gift None Pickup!"));
		return;
	}
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter == nullptr) {
		UE_LOG(LOG_CATEGORY_NAME, Error, TEXT("Pick Gift None Character!"));
		return;
	}
	pPickup->DoPickup(pCharacter);
}

bool AKillerPlayerController::Server_OnRemoveWeapon_Validate() {
	return true;
}
void AKillerPlayerController::Server_OnRemoveWeapon_Implementation() {
	AKillerCharacter* pCharacter = GetKillerCharacter();
	if (pCharacter != nullptr) {
		//pCharacter->RemoveWeapon();
	}
}
