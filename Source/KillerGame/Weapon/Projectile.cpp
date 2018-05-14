// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "kismet/GameplayStatics.h"
#include "BulletExplosionEffect.h"


// Sets default values
AProjectile::AProjectile()
	: bExploded(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	//CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->MaxSpeed = 2000.0f;
	//MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::PostInitializeComponents() {
	Super::PostInitializeComponents();

	MovementComp->OnProjectileStop.AddDynamic(this, &AProjectile::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(Instigator);
	AWeaponProjectile* OwnerWeapon = Cast<AWeaponProjectile>(GetOwner());
	if (OwnerWeapon) {
		ProjectileConfig = OwnerWeapon->GetProjectileConfig();
	}
	SetLifeSpan(ProjectileConfig.ProjectileLife);
}

void AProjectile::InitVelocity(FVector&ShootDir) {
	if (MovementComp) {
		MovementComp->Velocity = ShootDir * MovementComp->InitialSpeed;
	}
}

void AProjectile::OnImpact(const FHitResult& HitResult) {
	if (!bExploded) {
		DoExplode(HitResult);
		DisableAndDestroy(2.0f);
	}
}

void AProjectile::DoExplode(const FHitResult& HitResult) {
	if (ParticleComp) {
		ParticleComp->Deactivate();
	}
	// 以碰撞点为中心做爆炸
	const FVector ImpactLocation = HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f;
	//if ((ProjectileConfig.ExplosionDamage > 0)
	//	&& (ProjectileConfig.ExplosionRadius > 0)) {
	//	UGameplayStatics::ApplyRadialDamage(this
	//		, ProjectileConfig.ExplosionDamage
	//		, ImpactLocation
	//		, ProjectileConfig.ExplosionRadius
	//		, nullptr
	//		, TArray<AActor*>()
	//		, this
	//		, GetInstigatorController());
	//}
	if (m_BulletExplosionClass) {
		const FRotator SpawnRotation = HitResult.ImpactNormal.Rotation();
		ABulletExplosionEffect* EffectActor = GetWorld()->SpawnActorDeferred<ABulletExplosionEffect>(m_BulletExplosionClass
			, FTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint));
		if (EffectActor) {
			EffectActor->m_HitInfo = HitResult;
			UGameplayStatics::FinishSpawningActor(EffectActor
				, FTransform(SpawnRotation, ImpactLocation));
		}
	}
}

void AProjectile::DisableAndDestroy(float DelayTime) {
	if (MovementComp) {
		MovementComp->StopMovementImmediately();
	}
	SetLifeSpan(DelayTime);
}

bool AProjectile::ServerDoDamage_Validate(const FHitResult& HitResult) {
	return true;
}

void AProjectile::ServerDoDamage_Implementation(const FHitResult& HitResult) {
	// 以碰撞点为中心做爆炸
	const FVector ImpactLocation = HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f;
	if ((ProjectileConfig.ExplosionDamage > 0)
		&& (ProjectileConfig.ExplosionRadius > 0)) {
		UGameplayStatics::ApplyRadialDamage(this
			, ProjectileConfig.ExplosionDamage
			, ImpactLocation
			, ProjectileConfig.ExplosionRadius
			, nullptr
			, TArray<AActor*>()
			, this
			, GetInstigatorController());
	}
}
