// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/CapsuleComponent.h"
#include "Character/KillerCharacter.h"
#include "Player/KillerPlayerController.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UCapsuleComponent* pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickupCapsule"));
	RootComponent = pCapsule;
	pCapsule->InitCapsuleSize(40.0f, 50.0f);
	pCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	pCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	pCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	pCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PickupPSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PickupPSC"));
	PickupPSC->bAutoActivate = false;
	PickupPSC->bAutoDestroy = false;
	PickupPSC->SetupAttachment(RootComponent);
}

APickup::~APickup() {
}

//void APickup::BeginDestroy() {
//	if (PickupPSC) {
//		PickupPSC->Deactivate();
//	}
//}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	if (PickupPSC != nullptr) {
		PickupPSC->ActivateSystem();
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	AKillerCharacter*pPawn = Cast<AKillerCharacter>(OtherActor);
	if (pPawn != nullptr) {
		GiveGiftTo(pPawn);
	}
}

void APickup::GiveGiftTo(class AKillerCharacter* pPawn) {
	if (pPawn == nullptr) {
		return;
	}
	AKillerPlayerController* pc = Cast<AKillerPlayerController>(pPawn->GetController());
	if (pc != nullptr) {
		pc->PickupGift(this);
	}
}

void APickup::DestroyWithCleanup() {
	if (PickupPSC) {
		PickupPSC->Deactivate();
	}
	Destroy();
}
