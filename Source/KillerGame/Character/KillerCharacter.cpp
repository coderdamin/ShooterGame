// Fill out your copyright notice in the Description page of Project Settings.

#include "KillerCharacter.h"
#include "Weapon/Weapon.h"
#include "Engine/EngineTypes.h"
#include "KillerGame.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AKillerCharacter::AKillerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_bRuningState(false)
	, m_bAimingState(false)
	, m_bFiring(false)
	, m_nEquipedWeaponIndex(-1)
	, m_fTargetingSpeedModifier(0.2f)
	, m_fRunningSpeedModifier(1.8f)
	, m_fHP(100.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USkeletalMeshComponent* pMesh = GetMesh();
	check(pMesh);
	pMesh->bOnlyOwnerSee = false;
	pMesh->bOwnerNoSee = true;
	pMesh->bReceivesDecals = false;
	pMesh->SetCollisionObjectType(ECC_Pawn);

	m_pFPP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMesh"));
	check(m_pFPP);
	//m_pFPP->AttachTo(RootComponent);
	//m_pFPP->AttachToComponent(pMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	m_pFPP->SetupAttachment(RootComponent);
	m_pFPP->bOnlyOwnerSee = true;
	m_pFPP->bOwnerNoSee = false;
	m_pFPP->bCastDynamicShadow = false;
	m_pFPP->bReceivesDecals = false;
	m_pFPP->CastShadow = false;

	UCapsuleComponent* pCapsuleComponent = GetCapsuleComponent();
	check(pCapsuleComponent);
	pCapsuleComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	pCapsuleComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
	pCapsuleComponent->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Block);
}

void AKillerCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	//ConstructorHelpers::FClassFinder<AWeapon> WeaponClass(TEXT("Blueprint'/Game/Weapon/BP_WeaponTest.BP_WeaponTest'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponObject(TEXT("Blueprint'/Game/Weapon/BP_WeaponTest.BP_WeaponTest'"));
}

// Called when the game starts or when spawned
void AKillerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillerCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) {
	Super::CalcCamera(DeltaTime, OutResult);
}

void AKillerCharacter::OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation) {
	USkeletalMeshComponent* Mesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("FPPMesh")));
	const FMatrix MeshLS = FRotationTranslationMatrix(Mesh1P->RelativeRotation, Mesh1P->RelativeLocation);
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);

	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
	const FMatrix MeshRelativeToCamera = MeshLS * LeveledCameraLS.Inverse();
	const FMatrix PitchMesh = MeshRelativeToCamera * PitchedCameraLS;
	m_pFPP->SetRelativeLocationAndRotation(PitchMesh.GetOrigin(), PitchMesh.Rotator());
}

void AKillerCharacter::MoveForward(float fVal) {
	if (!FLOAT_EQUL(fVal, 0.0f)) {
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, fVal);
	}
}

void AKillerCharacter::MoveRight(float fVal) {
	if (!FLOAT_EQUL(fVal, 0.0f)) {
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, fVal);
	}
}

void AKillerCharacter::Turn(float fVal) {

}

void AKillerCharacter::LookUp(float fVal) {

}

void AKillerCharacter::Jump() {
	ACharacter::Jump();
}

void AKillerCharacter::StopJump() {
	ACharacter::StopJumping();
}

bool AKillerCharacter::CheckCanRun() {
	return (!m_bAimingState);
}
void AKillerCharacter::EnterRunningState() {
	m_bRuningState = true;
}

void AKillerCharacter::OutRunningState() {
	m_bRuningState = false;
}

void AKillerCharacter::EnterAimingState() {
	m_bAimingState = true;
}

void AKillerCharacter::OutAimingState() {
	m_bAimingState = false;
}

void AKillerCharacter::BeginFiring() {
	m_bFiring = true;
	AWeapon* pWeapon = GetCurrentWeapon();
	if (pWeapon != nullptr) {
		pWeapon->BeginFiring();
	}
}

void AKillerCharacter::EndFiring() {
	m_bFiring = false;
	AWeapon* pWeapon = GetCurrentWeapon();
	if (pWeapon != nullptr) {
		pWeapon->EndFiring();
	}
}

bool AKillerCharacter::IsFirstPerson() const {
	return IsAlive() && Controller && Controller->IsLocalPlayerController();
}

float AKillerCharacter::PlayAnimMontage(class UAnimMontage* pAnimMontage, float InPlayRate, FName StartSection) {
	USkeletalMeshComponent* pMesh = GetPawnMesh();
	if (pMesh && pAnimMontage && pMesh->AnimScriptInstance) {
		return pMesh->AnimScriptInstance->Montage_Play(pAnimMontage, InPlayRate);
	}
	return 0.0f;
}

void AKillerCharacter::StopAnimMontage(class UAnimMontage* pAnimMontage) {
	USkeletalMeshComponent* pMesh = GetPawnMesh();
	if (pMesh && pAnimMontage && pMesh->AnimScriptInstance
		&& pMesh->AnimScriptInstance->Montage_IsPlaying(pAnimMontage)) {
		pMesh->AnimScriptInstance->Montage_Stop(pAnimMontage->BlendOut.GetBlendTime());
	}
}

void AKillerCharacter::StopAllAnimMontage() {
	USkeletalMeshComponent* pMesh = GetPawnMesh();
	if (pMesh && pMesh->AnimScriptInstance) {
		pMesh->AnimScriptInstance->Montage_Stop(0.0f);
	}
}

void AKillerCharacter::PrevWeapon() {
	int nCount = m_WeaponArray.Num();
	if (nCount > 1) {
		if (m_nEquipedWeaponIndex == 0) {
			EquipWeaponByIndex(nCount - 1);
		}
		else if (m_nEquipedWeaponIndex >= 1){
			EquipWeaponByIndex(m_nEquipedWeaponIndex - 1);
		}
	}
}

void AKillerCharacter::NextWeapon() {
	int nCount = m_WeaponArray.Num();
	if (nCount > 1) {
		if (m_nEquipedWeaponIndex == nCount - 1) {
			EquipWeaponByIndex(0);
		}
		else {
			EquipWeaponByIndex(m_nEquipedWeaponIndex + 1);
		}
	}
}

void AKillerCharacter::ReloadAmmo() {
	AWeapon* pWeapon = GetCurrentWeapon();
	if (pWeapon != nullptr) {
		pWeapon->BeginReloadAmmo();
	}
}

void AKillerCharacter::SetBursting() {
	AWeapon* pWeapon = GetCurrentWeapon();
	if (pWeapon != nullptr) {
		pWeapon->SetBursting();
	}
}

AWeapon* AKillerCharacter::GetCurrentWeapon() {
	return GetWeapon(m_nEquipedWeaponIndex);
}

AWeapon* AKillerCharacter::GetWeapon(int nIndex) {
	AWeapon* pWeapon = nullptr;
	if (nIndex >= 0 && nIndex < WEAPON_MAX_COUNT && nIndex < m_WeaponArray.Num()) {
		pWeapon = m_WeaponArray[nIndex];
	}
	return pWeapon;
}

bool AKillerCharacter::PickWeapon(TSubclassOf<class AWeapon> weaponType) {
	if (WEAPON_MAX_COUNT > m_WeaponArray.Num()) {
		AWeapon* pWeapon = GetWorld()->SpawnActor<AWeapon>(weaponType);
		m_WeaponArray.Add(pWeapon);
		if (GetCurrentWeapon() == nullptr) {
			EquipWeaponByIndex(0);
		}
		if (Role == ROLE_Authority) {
			MulticastPickWeapon(weaponType);
		}
		return true;
	}
	return false;
}

bool AKillerCharacter::RemoveAllWeapon() {
	if (IsLocallyControlled()) {
		ServerRemoveAllWeapon();
	}
	else {
		for (auto Weapon : m_WeaponArray) {
			RemoveWeapon(Weapon);
		}
		m_WeaponArray.Empty();
		return true;
	}
	return false;
}

bool AKillerCharacter::RemoveWeapon(AWeapon*Weapon) {
	if (Weapon == nullptr) {
		return false;
	}
	Weapon->Unequip();
	int32 DelCount = m_WeaponArray.RemoveSingle(Weapon);
	return (DelCount > 0);
}

bool AKillerCharacter::RemoveWeaponByIndex(int nIndex) {
	if (IsLocallyControlled()) {
		ServerRemoveWeaponByIndex(nIndex);
	}else{
		if (nIndex >= 0 && nIndex < WEAPON_MAX_COUNT && nIndex < m_WeaponArray.Num()) {
			m_WeaponArray.RemoveAt(nIndex);
			return true;
		}
	}
	return false;
}

bool AKillerCharacter::EquipWeaponByIndex(int nIndex) {
	AWeapon* pWeapon = GetWeapon(nIndex);
	//if (pWeapon != nullptr && m_nEquipedWeaponIndex != nIndex) {
	if (pWeapon != nullptr){
		//if (Role != ROLE_Authority) {
			AWeapon* pWeaponLast = GetWeapon(m_nEquipedWeaponIndex);
			if (pWeaponLast != nullptr) {
				pWeaponLast->Unequip();
			}
			pWeapon->BeginEquip(this);
		//}
		//else {
			m_nEquipedWeaponIndex = nIndex;
		//}
		return true;
	}
	return false;
}

USkeletalMeshComponent* AKillerCharacter::GetSpecifcPawnMesh(bool bFirstPerson) {
	return bFirstPerson ? m_pFPP : GetMesh();
}

float AKillerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float FinalDamage = 0.0f;
	if ((Role == ROLE_Authority)
		|| (GetNetMode() != NM_Client)) {
		if (m_fHP > 0.0f) {
			FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
			if (FinalDamage > 0.0f) {
				m_fHP -= FinalDamage;
				//ReplicateHit(FinalDamage, DamageEvent, EventInstigator, DamageCauser, m_fHP <= 0.0f);
				if (m_fHP <= 0.0f) {
					m_fHP = 0.0f;
				}
			}
		}
		//if (Role != ROLE_Authority) {
			if (m_fHP <= 0.0f) {
				PlayDie(FinalDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : nullptr, DamageCauser);
			}
			else {
				PlayHit(FinalDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : nullptr, DamageCauser);
			}
		//}
		MulticastPlayDamage(m_fHP, DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else {
		ServerTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return FinalDamage;
}

void AKillerCharacter::PlayHit(float DamageAmount, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, AActor* DamageCauser) {
	if (DamageAmount > 0.0f) {
		ApplyDamageMomentum(DamageAmount, DamageEvent, PawnInstigator, DamageCauser);
	}
	// Notify Blueprint

}

void AKillerCharacter::PlayDie(float DamageAmount, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, AActor* DamageCauser) {
	RemoveAllWeapon();
	StopAllAnimMontage();
	ChangeCameraMode(EM_CAMERAMODE_THIRD);
	UCapsuleComponent* pCapsuleComponent = GetCapsuleComponent();
	if (pCapsuleComponent != nullptr) {
		pCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		pCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	UCharacterMovementComponent* pCharacterMovement = GetCharacterMovement();
	if (pCharacterMovement != nullptr) {
		pCharacterMovement->StopMovementImmediately();
		pCharacterMovement->DisableMovement();
		pCharacterMovement->SetComponentTickEnabled(false);
	}
	float DeathAnimDuration = PlayAnimMontage(DeathAnim);
	if (DeathAnimDuration > 0.0f) {
		GetWorldTimerManager().SetTimer(m_hDeathAnimTimer
			, this
			, &AKillerCharacter::DeathAnimCompleted
			, DeathAnimDuration
			, false
		);
	}
	else {
		SetRagdollPhysics();
	}
}

void AKillerCharacter::ChangeCameraMode(CAMERAMODE cm) {
	switch (cm) {
	case EM_CAMERAMODE_FIRST:
	{
		m_pFPP->SetOwnerNoSee(false);
		m_pFPP->SetOnlyOwnerSee(true);
		USkeletalMeshComponent* pMesh = GetMesh();
		pMesh->SetOwnerNoSee(true);
		pMesh->SetOnlyOwnerSee(false);
		pMesh->SetHiddenInGame(true);
		m_pFPP->SetHiddenInGame(false);
		break; 
	}
	case EM_CAMERAMODE_THIRD:
	{
		m_pFPP->SetOwnerNoSee(true);
		m_pFPP->SetOnlyOwnerSee(true);
		USkeletalMeshComponent* pMesh = GetMesh();
		pMesh->SetOwnerNoSee(false);
		pMesh->SetOnlyOwnerSee(false);
		pMesh->SetHiddenInGame(false);
		m_pFPP->SetHiddenInGame(true);
		break;
	}
	default:
		printf("[AKillerCharacter, ChangeCameraMode]Unknow CameraMode %d", cm);
		break;
	}
}

void AKillerCharacter::DeathAnimCompleted() {
	if (m_hDeathAnimTimer.IsValid()) {
		GetWorldTimerManager().ClearTimer(m_hDeathAnimTimer);
	}
	SetRagdollPhysics();
}

void AKillerCharacter::SetRagdollPhysics() {
	USkeletalMeshComponent* pMesh = GetMesh();
	if (pMesh == nullptr) {
		return;
	}
	pMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	SetActorEnableCollision(true);
	pMesh->SetAllBodiesSimulatePhysics(true);
	pMesh->SetSimulatePhysics(true);
	pMesh->WakeAllRigidBodies();
	pMesh->bBlendPhysics = true;
}

FRotator AKillerCharacter::GetAimOffsets() const {
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimdirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	return AimdirLS.Rotation();
}

void AKillerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AKillerCharacter, m_fHP, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AKillerCharacter, m_nEquipedWeaponIndex, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AKillerCharacter, m_bRuningState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AKillerCharacter, m_bAimingState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AKillerCharacter, m_bFiring, COND_SkipOwner);
}

void AKillerCharacter::OnRep_EquipedWeaponIndex(int nWeaponIndex) {
	//if (Role == ROLE_SimulatedProxy) {
	this->EquipWeaponByIndex(nWeaponIndex);
	//}
}

bool AKillerCharacter::ServerPickWeapon_Validate(TSubclassOf<class AWeapon> weaponType) {
	return true;
}

void AKillerCharacter::ServerPickWeapon_Implementation(TSubclassOf<class AWeapon> weaponType) {
	bool bRet = PickWeapon(weaponType);
	if (bRet) {
		MulticastPickWeapon(weaponType);
	}
}

void AKillerCharacter::MulticastPickWeapon_Implementation(TSubclassOf<AWeapon> weaponType) {
	//if ((Role != ROLE_Authority) &&(!IsLocallyControlled())) {
	//if (Role != ROLE_Authority) {
	PickWeapon(weaponType);
	//}
}

bool AKillerCharacter::ServerRemoveWeaponByIndex_Validate(int nIndex) {
	return true;
}

void AKillerCharacter::ServerRemoveWeaponByIndex_Implementation(int nIndex) {
	MulticastRemoveWeaponByIndex(nIndex);
}

void AKillerCharacter::MulticastRemoveWeaponByIndex_Implementation(int nIndex) {
	if (Role != ROLE_Authority) {
		RemoveWeaponByIndex(nIndex);
	}
}

bool AKillerCharacter::ServerRemoveAllWeapon_Validate() {
	return true;
}

void AKillerCharacter::ServerRemoveAllWeapon_Implementation() {
	RemoveAllWeapon();
}

void AKillerCharacter::MulticastRemoveAllWeapon_Implementation() {
	if (Role != ROLE_Authority) {
		MulticastRemoveAllWeapon();
	}
}

bool AKillerCharacter::ServerTakeDamage_Validate(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	return true;
}

void AKillerCharacter::ServerTakeDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	float FinalDamage = TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AKillerCharacter::MulticastPlayDamage_Implementation(float hp, float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (Role != ROLE_Authority) {
		if (hp <= 0.0f) {
			PlayDie(DamageAmount, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : nullptr, DamageCauser);
		}
		else {
			PlayHit(DamageAmount, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : nullptr, DamageCauser);
		}
	}
}
