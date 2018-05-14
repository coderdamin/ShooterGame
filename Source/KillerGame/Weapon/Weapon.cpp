// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Character/KillerCharacter.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "AIController.h"
#include "KillerGame.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeapon::AWeapon()
	: m_emState(WeaponState_None)
	, m_pOwner(nullptr)
	, m_bBursting(true)
	, m_pMuzzleFX(nullptr)
	, m_pMuzzlePSC(nullptr)
	, m_fLastFireTime(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	//RootComponent = CapsuleComponent;
	////CapsuleComponent->InitCapsuleSize(40.0f, 50.0f);
	//CapsuleComponent->SetCollisionObjectType(ECC_WorldDynamic);
	//CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//CapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	//CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	m_pMesh3p = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon3P"));
	check(m_pMesh3p);
	//m_pMesh3p->SetupAttachment(RootComponent);
	RootComponent = m_pMesh3p;
	m_pMesh3p->bOnlyOwnerSee = false;
	m_pMesh3p->bOwnerNoSee = true;
	m_pMesh3p->bReceivesDecals = false;
	m_pMesh3p->bCastDynamicShadow = false;
	m_pMesh3p->CastShadow = true;
	m_pMesh3p->SetCollisionObjectType(ECC_WorldDynamic);
	m_pMesh3p->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pMesh3p->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_pMesh3p->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	m_pMesh1p = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon1P"));
	//m_pMesh1p->AttachTo(RootComponent);
	//m_pMesh1p->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	m_pMesh1p->SetupAttachment(RootComponent);
	m_pMesh1p->bOnlyOwnerSee = true;
	m_pMesh1p->bOwnerNoSee = false;
	m_pMesh1p->bReceivesDecals = false;
	m_pMesh1p->bCastDynamicShadow = false;
	m_pMesh1p->CastShadow = false;
	m_pMesh1p->SetCollisionObjectType(ECC_WorldDynamic);
	m_pMesh1p->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pMesh1p->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	UpdateState(WeaponState_Idle);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Destroyed() {
	Super::Destroyed();
}

USkeletalMeshComponent* AWeapon::GetWeaponMesh() {
	return (m_pOwner != nullptr && m_pOwner->IsFirstPerson()) ? m_pMesh1p : m_pMesh3p;
}

void AWeapon::DetachMeshFronPawn() {
	m_pMesh1p->SetHiddenInGame(true);
	m_pMesh3p->SetHiddenInGame(true);
	m_pMesh1p->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	m_pMesh3p->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon::AttachMeshToPawn(AKillerCharacter* pOwner) {
	check(pOwner);
	DetachMeshFronPawn();
	FName AttachPoint = pOwner->GetWeaponAttachPointName();
	USkeletalMeshComponent*pPawnMesh1p = pOwner->GetSpecifcPawnMesh(true);
	USkeletalMeshComponent*pPawnMesh3p = pOwner->GetSpecifcPawnMesh(false);
	m_pMesh1p->SetHiddenInGame(false);
	m_pMesh3p->SetHiddenInGame(false);
	m_pMesh1p->AttachToComponent(pPawnMesh1p, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	m_pMesh3p->AttachToComponent(pPawnMesh3p, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
}

bool AWeapon::BeginEquip(AKillerCharacter* pOwner) {
	check(pOwner);
	if (pOwner != m_pOwner) {
		m_pOwner = pOwner;
		Instigator = pOwner;
		SetOwner(m_pOwner);
	}
	//AttachMeshToPawn(pOwner);
	bool bResult = UpdateState(WeaponState_Equiping);
	//if (!bResult) {
	//	DetachMeshFronPawn();
	//}
	return bResult;
}

bool AWeapon::Unequip() {
	GetWorldTimerManager().ClearTimer(m_hEquipTimer);
	OnEquipCompleted();

	DetachMeshFronPawn();
	m_pOwner = nullptr;
	SetOwner(m_pOwner);

	UpdateState(WeaponState_Idle);
	return true;
}

bool AWeapon::BeginFiring() {
	return UpdateState(WeaponState_Firing);
}

bool AWeapon::EndFiring() {
	GetWorldTimerManager().ClearTimer(m_hFireTimer);
	EndSimulateStrafe();
	return UpdateState(WeaponState_Idle);
}

bool AWeapon::BeginReloadAmmo() {
	return UpdateState(WeaponState_Reloading);
}

bool AWeapon::EndReloadAmmo() {
	GetWorldTimerManager().ClearTimer(m_hReloadAmmoTimer);
	OnReloadAmmoCompleted();
	return UpdateState(WeaponState_Idle);
}

bool AWeapon::AddAmmo(int32 nAmmoCount) {
	if (CheckCanAddAmmo()) {
		m_nCurrentAmmo += FMath::Max(nAmmoCount, 0);
		return true;
	}
	return false;
}

bool AWeapon::CheckAmmoType(EMWeaponAmmoType emType) {
	return (AmmoType == emType);
}

const FVector AWeapon::GetMuzzleLocation() {
	USkeletalMeshComponent* pWeaponMesh = GetWeaponMesh();
	if (pWeaponMesh != nullptr) {
		return pWeaponMesh->GetSocketLocation(m_MuzzleAttachPoint);
	}
	return FVector::ZeroVector;
}

bool AWeapon::UpdateState(EMWeaponState emState) {
	if (emState == m_emState) {
		return false;
	}
	EMWeaponState emNewState = WeaponState_None;
	if (m_emState == WeaponState_Idle) {
		switch (emState) {
		case WeaponState_Idle:
			emNewState = WeaponState_Idle;
			break;
		case WeaponState_Firing:
			if (CheckCanFire()) {
				emNewState = WeaponState_Firing;
			}
			break;
		case WeaponState_Equiping:
			if (CheckCanEquip()) {
				emNewState = WeaponState_Equiping;
			}
			break;
		case WeaponState_Reloading:
			if (CheckCanReloadAmmo()) {
				emNewState = WeaponState_Reloading;
			}
			break;
		default:
			emNewState = WeaponState_Idle;
			break;
		}
	}
	else if (emState == WeaponState_Idle) {
		emNewState = WeaponState_Idle;
	}
	if ((emNewState != WeaponState_None)
		&& (emNewState != m_emState)){
		// Do State Changed
		m_emState = TEnumAsByte<EMWeaponState>(emNewState);
		switch (m_emState) {
		case WeaponState_Firing:
		{
			// time check
			const float fGameTime = GetWorld()->GetTimeSeconds();
			if (fGameTime - m_fLastFireTime >= m_WeaponConfig.fFireRate) {
				HandleFiring();
			}
			else {
				GetWorldTimerManager().SetTimer(m_hFireTimer, this, &AWeapon::HandleFiring
					, m_fLastFireTime + m_WeaponConfig.fFireRate - fGameTime, false);
			}
		}
			break;
		case WeaponState_Equiping:
		{
			GetWorldTimerManager().SetTimer(m_hEquipTimer, this, &AWeapon::OnEquipCompleted, m_WeaponConfig.fArmingTime, false);
			AttachMeshToPawn(m_pOwner);
			PlayWeaponAnimation(m_EquipAnim);
		}
			break;
		case WeaponState_Reloading:
		{
			const uint32 nReloadCount = FMath::Min(m_WeaponConfig.nMagazineSize, m_nCurrentAmmo - m_nCurrentAmmoInClip);
			const float fReloadTime = m_WeaponConfig.fReloadSpeed - 0.8 * m_WeaponConfig.fReloadSpeed * (m_nCurrentAmmoInClip / nReloadCount);
			GetWorldTimerManager().SetTimer(m_hReloadAmmoTimer, this, &AWeapon::OnReloadAmmoCompleted, fReloadTime, false);
			PlayWeaponAnimation(m_ReloadAmmoAnim);
		}
			break;
		case WeaponState_Idle:
		default:
			//DoIdle();
			break;
		}
		return true;
	}
	return false;
}

//void AWeapon::DoStateChanged() {
//}

void AWeapon::HandleFiring() {
	if (!CheckCanFire()) {
		// PlaySound();
		EndFiring();
		return;
	}
	SimulateStrafe();
	// 不同枪射击的不同处理
	OnFiring();
	m_nCurrentAmmoInClip -= 1;
	// HUD刷新

	if (m_bBursting) {
		EndFiring();
	}
	else {
		GetWorldTimerManager().SetTimer(m_hFireTimer, this, &AWeapon::HandleFiring, m_WeaponConfig.fFireRate, false);
	}
	m_fLastFireTime = GetWorld()->GetTimeSeconds();
}

void AWeapon::SimulateStrafe() {
	// 射击特效、人物动作、射击音效、摄像头震动
	//if (Role == ROLE_Authority) {
	//	return;
	//}

	if (m_pMuzzlePSC == nullptr) {
		USkeletalMeshComponent* pWeaponMesh = GetWeaponMesh();
		if (pWeaponMesh != nullptr) {
			m_pMuzzlePSC = UGameplayStatics::SpawnEmitterAttached(m_pMuzzleFX, pWeaponMesh, m_MuzzleAttachPoint);
		}
	}

	PlayWeaponAnimation(m_FireAnim);
	//UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());

	APlayerController* pc = m_pOwner ? Cast<APlayerController>(m_pOwner->Controller) : nullptr;
	if (pc && m_FireCameraShake) {
		pc->ClientPlayCameraShake(m_FireCameraShake, 1.0f);
	}
}

void AWeapon::EndSimulateStrafe() {
	if (m_pMuzzlePSC) {
		m_pMuzzlePSC->DeactivateSystem();
		m_pMuzzlePSC = nullptr;
	}

	StopWeaponAnimation(m_FireAnim);
}

float AWeapon::PlayWeaponAnimation(FWeaponAnim&Anim) {
	if (m_pOwner == nullptr) {
		return 0.0f;
	}
	UAnimMontage* pUseAnim = m_pOwner->IsFirstPerson() ? Anim.Anim1P : Anim.Anim3P;
	return m_pOwner->PlayAnimMontage(pUseAnim);
}

void AWeapon::StopWeaponAnimation(FWeaponAnim&Anim) {
	if (m_pOwner == nullptr) {
		return;
	}
	UAnimMontage* pUseAnim = m_pOwner->IsFirstPerson() ? Anim.Anim1P : Anim.Anim3P;
	m_pOwner->StopAnimMontage(pUseAnim);
}

void AWeapon::OnEquipCompleted() {
	StopWeaponAnimation(m_EquipAnim);
	UpdateState(WeaponState_Idle);
}

void AWeapon::OnReloadAmmoCompleted() {
	StopWeaponAnimation(m_ReloadAmmoAnim);
	int32 nNeed = m_WeaponConfig.nMagazineSize - m_nCurrentAmmoInClip;
	if (m_nCurrentAmmo > nNeed) {
		m_nCurrentAmmo = m_nCurrentAmmo - nNeed;
		m_nCurrentAmmoInClip = m_WeaponConfig.nMagazineSize;
	}
	else {
		m_nCurrentAmmoInClip += m_nCurrentAmmo;
		m_nCurrentAmmo = 0;
	}
	UpdateState(WeaponState_Idle);
}

bool AWeapon::CheckCanFire() {
	return (m_nCurrentAmmoInClip > 0);
}

bool AWeapon::CheckCanEquip() {
	return true;
}

bool AWeapon::CheckCanReloadAmmo() {
	return (m_nCurrentAmmo > 0);
}

bool AWeapon::CheckCanAddAmmo() {
	return true;
}

FVector AWeapon::GetAdjustedAim() {
	FVector RetAim = FVector::ZeroVector;
	const APlayerController* pPlayerControl = Instigator ? Cast<APlayerController>(Instigator->GetController()) : nullptr;
	if (pPlayerControl) {
		FVector CamLocation;
		FRotator CamRotation;
		// 如果要支持control方向不同于摄像机方向，此处应用control方向
		//GetPlayerViewPoint[out_Location][out_Rotation] - 这个函数返回了Controller的Pawn的视角。对于人类玩家来说，这是指相机的视角。对于AI - 控制的玩家来说，这个函数是Pawn眼中的视角。在这个基本的实现过程中，它只是 Controller 自己的位置和旋转量。
		//GetActorEyesViewPoint[out_Location][out_Rotation] - 如果存在Controller或它的Pawn，这个函数则返回Controller或他的Pawn的视角。基本上，它会返回玩家从哪个位置和方向观看。
		//IsAimingAt[Target][Epsilon] - 这个函数返回在给定的变动幅度内，这个Controller当前是否正在朝着指定的目标瞄准。Epsilon为1.0意味着直接朝目标瞄准，但是较低的值允许一些误差。
		pPlayerControl->GetPlayerViewPoint(CamLocation, CamRotation);
		RetAim = CamRotation.Vector();
	}
	else {
		//AAIController* pAIController = m_pOwner ? Cast<AAIController>(m_pOwner->GetController()) : nullptr;
		//if (pAIController) {
		//	RetAim = pAIController->GetControlRotation().Vector();
		//}
		//else {
		RetAim = Instigator->GetBaseAimRotation().Vector();
		//}
	}
	return RetAim;
}

FVector AWeapon::GetCameraDamageStartLocation(const FVector&AimDir) {
	FVector StartTrace = FVector::ZeroVector;
	const APlayerController* pPlayerControl = Instigator ? Cast<APlayerController>(Instigator->GetController()) : nullptr;
	if (pPlayerControl) {
		FRotator UnusedRot;
		pPlayerControl->GetPlayerViewPoint(StartTrace, UnusedRot);
		StartTrace = StartTrace + AimDir * ((Instigator->GetActorLocation() - StartTrace) | AimDir);
	}
	else {
		StartTrace = Instigator->GetActorLocation();
	}
	return StartTrace;
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeapon, m_nCurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeapon, m_nCurrentAmmoInClip, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AWeapon, m_emState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AWeapon, m_bBursting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AWeapon, m_fLastFireTime, COND_SkipOwner);
}

void AWeapon::OnRep_WeaponState(TEnumAsByte<EMWeaponState> emState) {
	if (Role == ROLE_SimulatedProxy) {
		UpdateState(EMWeaponState(emState));
	}
}