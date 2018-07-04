// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Array.h"
#include "KillerCharacter.generated.h"

UENUM()
enum CAMERAMODE {
	EM_CAMERAMODE_FIRST,
	EM_CAMERAMODE_THIRD,
	EM_CAMERAMODE_MAX,
};

UCLASS()
class KILLERGAME_API AKillerCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
	//GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKillerCharacter();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayDie(float DamageAmount, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, AActor* DamageCauser);
	void PlayHit(float DamageAmount, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, AActor* DamageCauser);
	void SetRagdollPhysics();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// Called to bind functionality to input
	// modify 玩家操作由KillerPlayerController接收
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	float PlayAnimMontage(class UAnimMontage* pAnimMontage, float InPlayRate = 1.0f, FName StartSection = NAME_None);
	void StopAnimMontage(class UAnimMontage* pAnimMontage);
	void StopAllAnimMontage();
	FName& GetWeaponAttachPointName() { return m_WeaponAttachPoint; }
	USkeletalMeshComponent* GetSpecifcPawnMesh(bool bFirstPerson);
	float GetTargetingSpeedModifier() const { return m_fTargetingSpeedModifier; }
	float GetRunningSpeedModifier() const { return m_fRunningSpeedModifier; }
	void OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation);
	void ChangeCameraMode(CAMERAMODE cm);
	void DeathAnimCompleted();

public:
	UFUNCTION(BlueprintCallable, Category="Game|Character")
	bool IsRuningState() const { return m_bRuningState; }

	UFUNCTION(BlueprintCallable, Category="Game|Character")
	bool IsAimingState() const { return m_bAimingState; }

	UFUNCTION(BlueprintCallable, Category="Game|Combar")
	bool IsFiring() const { return m_bFiring; }

	UFUNCTION(BlueprintCallable, Category = "Game|Character")
	virtual bool IsFirstPerson() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Character")
	USkeletalMeshComponent* GetPawnMesh() { return IsFirstPerson() ? m_pFPP : GetMesh(); }

	UFUNCTION(BlueprintCallable, Category = "Game|Character")
	bool IsAlive() const { return ((m_fHP > 0.0f) && (!FMath::IsNearlyZero(m_fHP))); }
	
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	TArray<AWeapon *> GetWeaponList() { return m_WeaponArray; }

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	AWeapon* GetWeapon(int nIndex = 0);

	UFUNCTION(BlueprintCallable, Category="Game|Weapon")
	AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool PickWeapon(TSubclassOf<class AWeapon> weaponType);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool RemoveWeaponByIndex(int nIndex);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool RemoveWeapon(AWeapon*Weapon);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool RemoveAllWeapon();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool EquipWeaponByIndex(int nIndex);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool GetEquipedWeaponIndex() { return m_nEquipedWeaponIndex; }

	UFUNCTION(BlueprintCallable, Category = "Game|Combar")
	FRotator GetAimOffsets() const;

	UFUNCTION()
	void OnRep_EquipedWeaponIndex(int nWeaponIndex);

	//// 服务的过来的操作，
	//UFUNCTION(reliable, NetMulticast)
	//void Multicast_OnReloadAmmo();

	//UFUNCTION(reliable, NetMulticast)
	//void Multicast_OnPrevWeapon();

	//UFUNCTION(reliable, NetMulticast)
	//void Multicast_OnNextWeapon();

public:
	void MoveForward(float fVal);
	void MoveRight(float fVal);
	void Jump();
	void StopJump();
	bool CheckCanRun();
	void EnterRunningState();
	void OutRunningState();
	void EnterAimingState();
	void OutAimingState();
	void BeginFiring();
	void EndFiring();
	void NextWeapon();
	void PrevWeapon();
	void ReloadAmmo();
	void SetBursting();
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character)
	USkeletalMeshComponent * m_pFPP;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnim;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float m_fTargetingSpeedModifier;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float m_fRunningSpeedModifier;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName m_WeaponAttachPoint;

	UPROPERTY(Transient, Replicated)
	float m_fHP;
	
	UPROPERTY(Transient, ReplicatedUsing = OnRep_EquipedWeaponIndex)
	int m_nEquipedWeaponIndex;

	UPROPERTY(Transient, Replicated)
	bool m_bRuningState;

	UPROPERTY(Transient, Replicated)
	bool m_bAimingState;

	UPROPERTY(Transient, Replicated)
	bool m_bFiring;

private:
	UPROPERTY(Transient, Replicated)
	TArray<class AWeapon *> m_WeaponArray;

	FTimerHandle m_hDeathAnimTimer;
};
