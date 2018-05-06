// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/CapsuleComponent.h"
#include "Character/KillerCharacter.h"


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
		DoPickup(pPawn);
	}
}

void APickup::DoPickup(class AKillerCharacter* pPawn) {
	if (GiveGiftTo(pPawn)) {
		if (PickupPSC) {
			PickupPSC->Deactivate();
		}
		Destroy();
	}
}

bool APickup::GiveGiftTo(class AKillerCharacter* pPawn) {
	return true;
}

