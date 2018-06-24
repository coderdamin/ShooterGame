// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KillerPlayerController.generated.h"

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
	bool OnCheckCanRun();
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

public:
	class AKillerCharacter* GetKillerCharacter();

	// TODO:Client To Server������ͬ����Ϊͬ����ҵ�1����������/2�����������������״̬
};
