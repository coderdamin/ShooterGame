// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KillerPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_CATEGORY_NAME, Log, All);

/**
 * 
 */
UCLASS()
class KILLERGAME_API AKillerPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
		
public:
	virtual void SetupInputComponent() override;

public:
	void OnMoveForward(float fVal);
	void OnMoveRight(float fVal);
	void OnTurn(float fVal);
	void OnLookUp(float fVal);
	void OnJump();
	void OnStopJump();
	void OnEnterRunningState();
	void OnOutRunningState();
	void OnEnterAimingState();
	void OnOutAimingState();
	void OnBeginFiring();
	void OnEndFiring();
	void OnNextWeapon();
	void OnPrevWeapon();
	void OnReloadAmmo();
	void OnSetBursting();
	void OnOpenScoreBoard();
	void OnOpenChatInput();

	void PickupGift(class APickup* pPickup);

public:
	UFUNCTION(reliable, server, WithValidation)
	void Server_OnEnterRunningState(bool bEnter);

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnEnterAimingState(bool bEnter);

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnBeginFiring(bool bBegin);

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnNextWeapon(bool bNext);

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnSetBursting();

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnReloadAmmo();

	/// ///
	UFUNCTION(reliable, server, WithValidation)
	void Server_OnTakeDamage();

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnPickupGift(class APickup* pPickup);

	UFUNCTION(reliable, server, WithValidation)
	void Server_OnRemoveWeapon();
	
public:
	class AKillerCharacter* GetKillerCharacter();

private:
};
