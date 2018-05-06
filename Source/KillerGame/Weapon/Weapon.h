// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObjectGlobals.h"
#include "CommonTypes.h"
#include "Weapon.generated.h"

UENUM()
enum EMWeaponState {
	WeaponState_None,
	WeaponState_Idle,
	WeaponState_Firing,
	WeaponState_Reloading,
	WeaponState_Equiping,
};

USTRUCT()
struct FWeaponAnim {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* Anim1P;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* Anim3P;

	FWeaponAnim() : Anim1P(nullptr)
		, Anim3P(nullptr)
	{}
};

USTRUCT()
struct FWeaponConfig {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fFireRate;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fReloadSpeed;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fArmingTime;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fWeaponRange;
	
	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	float fDamage = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	int32 nMagazineSize;

	//UPROPERTY(EditDefaultsOnly, Category = WeaponConfig)
	//uint32 nLife;

	FWeaponConfig() : fFireRate(0.5f)
		, fReloadSpeed(3.0f)
		, fArmingTime(5.0f)
		, nMagazineSize(35)
		, fWeaponRange(1000.0f)
	{}
};

UCLASS()
class KILLERGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	USkeletalMeshComponent* GetWeaponMesh();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool BeginEquip(AKillerCharacter* pOwner);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool Unequip();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool BeginFiring();
	
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool EndFiring();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool BeginReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool EndReloadAmmo();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool AddAmmo(int32 nAmmoCount);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	const FVector GetMuzzleLocation();

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool CheckAmmoType(EMWeaponAmmoType emType);

	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	void SetBursting() {
		m_bBursting = !m_bBursting;
	}

	UFUNCTION()
	void OnRep_WeaponState(TEnumAsByte<EMWeaponState> emState);

protected:
	virtual void OnFiring() {}
	bool UpdateState(EMWeaponState emState);
	void HandleFiring();
	void SimulateStrafe();
	void EndSimulateStrafe();
	float PlayWeaponAnimation(FWeaponAnim& Anim);
	void StopWeaponAnimation(FWeaponAnim& Anim);
	void OnEquipCompleted();
	void OnReloadAmmoCompleted();
	FVector GetAdjustedAim();
	FVector GetCameraDamageStartLocation(const FVector&AimDir);
	
private:
	void AttachMeshToPawn(AKillerCharacter* pOwner);
	void DetachMeshFronPawn();
	//void DoStateChanged();
	bool CheckCanFire();
	bool CheckCanEquip();
	bool CheckCanReloadAmmo();
	bool CheckCanAddAmmo();

public:
	//UPROPERTY(EditDefaultsOnly, Category = Capsule)
	//UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* m_pMesh1p;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* m_pMesh3p;
	// SubobjectPtr<USkeletalMeshComponent> m_Mesh3p;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim m_FireAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim m_EquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim m_ReloadAmmoAnim;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class UCameraShake> m_FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName m_MuzzleAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* m_pMuzzleFX;

	UPROPERTY()
	UParticleSystemComponent* m_pMuzzlePSC;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FWeaponConfig m_WeaponConfig;

	UPROPERTY(BlueprintReadOnly, Category = Weapon, Transient, Replicated)
	int32 m_nCurrentAmmo;

	UPROPERTY(BlueprintReadOnly, Category = Weapon, Transient, Replicated)
	int32 m_nCurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TEnumAsByte<EMWeaponAmmoType> AmmoType;

protected:
	AKillerCharacter * m_pOwner;

private:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_WeaponState)
	TEnumAsByte<EMWeaponState> m_emState;

	UPROPERTY(Transient, Replicated)
	bool m_bBursting;		// µ„…‰
	// shooteTimer
	UPROPERTY(Transient, Replicated)
	float m_fLastFireTime;

	FTimerHandle m_hFireTimer;
	FTimerHandle m_hEquipTimer;
	FTimerHandle m_hReloadAmmoTimer;
};

