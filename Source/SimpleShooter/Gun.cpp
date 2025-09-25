// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return;
	}
	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController)
	{
		return;
	}
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	const FVector End = Location + Rotation.Vector() * MaxRange;
	FHitResult Hit;

	// For ignoring self hit
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	const bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
	if (!bSuccess)
	{
		return;
	}

	const FVector ShotDirection = -Rotation.Vector();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return;
	}

	HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

