// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponInstant.h"
#include "Character/KillerCharacter.h"
#include "Engine/EngineTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "kismet/GameplayStatics.h"
#include "BulletImpactEffect.h"

AWeaponInstant::AWeaponInstant()
	: m_fCurrentSpread(1.0f)
{}

FHitResult AWeaponInstant::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) {
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = false;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);
	return Hit;
}

void AWeaponInstant::OnFiring() {
	const FVector AimDir = GetAdjustedAim();
	const FVector StartTrace = GetCameraDamageStartLocation(AimDir);
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = GetCurrentSpread();
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, ConeHalfAngle, ConeHalfAngle);
	const FVector EndTrace = StartTrace + ShootDir * m_WeaponConfig.fWeaponRange;

	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);
	ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);

	RefreshCurrentSpread();
}

bool AWeaponInstant::EndFiring() {
	bool bRet = Super::EndFiring();
	RefreshCurrentSpread(true);
	return bRet;
}

float AWeaponInstant::GetCurrentSpread() const {
	return m_fCurrentSpread;
}

void AWeaponInstant::RefreshCurrentSpread(bool bForceInit) {
	if (bForceInit) {
		m_fCurrentSpread = m_InstantConfig.fFiringSpreadInit;
	}
	else {
		m_fCurrentSpread = FMath::Min(m_InstantConfig.fFiringSpreadMax
				, m_fCurrentSpread + m_InstantConfig.fFiringSpreadIncrement);
	}
}

void AWeaponInstant::ProcessInstantHit(const FHitResult&Impact, const FVector&Origin, const FVector&ShootDir, int32 RandomSeed, float Reticlespread) {
	// �������
	ProcessInstantHit_Confirmed(Impact, Origin, ShootDir, RandomSeed, Reticlespread);
}

// RandomSeed, Reticlespread ����ͬ���������ͻ��ˣ����������
void AWeaponInstant::ProcessInstantHit_Confirmed(const FHitResult&Impact, const FVector&Origin, const FVector&ShootDir, int32 RandomSeed, float Reticlespread) {
	// ͬ���������ͻ���
	//if (Role == ROLE_Authority) {
	////	HitNotify = 
	//}
	// �����ͻ���/�����
	if (m_pOwner) {
		if ((m_pOwner->Role == ROLE_Authority)
			|| (GetNetMode() != NM_Client)) {
			DoDamage(Impact, ShootDir);
		}
		else {
			ServerDoDamage(Impact, ShootDir);
		}	
	}
	// Effect
	if (GetNetMode() != NM_DedicatedServer) {
		const FVector EndPoint = Origin + ShootDir * m_WeaponConfig.fWeaponRange;
		SpawnTrailEffect(EndPoint);
		SpawnImpactEffects(Impact);
	}
}

// �����ͻ���/�����
void AWeaponInstant::DoDamage(const FHitResult&Impact, const FVector&ShootDir) {
	AActor* pActor = Impact.GetActor();
	if (pActor && pActor->bTearOff) {
		FPointDamageEvent PointDamage;
		PointDamage.HitInfo = Impact;
		PointDamage.ShotDirection = ShootDir;
		PointDamage.Damage = m_WeaponConfig.fDamage;
		pActor->TakeDamage(PointDamage.Damage
			, PointDamage
			, (m_pOwner != nullptr) ? m_pOwner->GetController() : nullptr
			, this
		);
	}
}

void AWeaponInstant::SpawnTrailEffect(const FVector&EndPoint) {
	if (m_pTrailFX != nullptr) {
		const FVector Origin = GetMuzzleLocation();
		UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(this, m_pTrailFX, Origin);
		if (TrailPSC) {
			TrailPSC->SetVectorParameter(m_TrailTargetParam, EndPoint);
		}
	}
}

void AWeaponInstant::SpawnImpactEffects(const FHitResult&Impact) {
	if ((m_BulletImpactEffectClass != nullptr) && (Impact.bBlockingHit)) {
		ABulletImpactEffect* pEffectActor = GetWorld()->SpawnActorDeferred<ABulletImpactEffect>(m_BulletImpactEffectClass
			, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		if (pEffectActor != nullptr) {
			pEffectActor->m_HitInfo = Impact;
			UGameplayStatics::FinishSpawningActor(pEffectActor
				, FTransform(Impact.ImpactNormal.Rotation(), Impact.ImpactPoint));
		}
	}
}

void AWeaponInstant::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProperty) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProperty);
}

bool AWeaponInstant::ServerDoDamage_Validate(const FHitResult&Impact, const FVector&ShootDir) {
	return true;
}

void AWeaponInstant::ServerDoDamage_Implementation(const FHitResult&Impact, const FVector&ShootDir) {
	DoDamage(Impact, ShootDir);
}