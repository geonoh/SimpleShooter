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
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	const bool bSuccess = GunTrace(Hit, ShotDirection);
	if (!bSuccess)
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
	
	FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);

	AActor* HitActor = Hit.GetActor();
	if (!HitActor)
	{
		return;
	}

	HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
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

bool AGun::GunTrace(FHitResult& OutHitResult, FVector& OutShotDirection) const
{
	const AController* Controller = GetOwnerController();
	if (!Controller)
	{
		return false;
	}
	
	FVector Location;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Location, Rotation);
	OutShotDirection = -Rotation.Vector();
	const FVector End = Location + Rotation.Vector() * MaxRange;
	// For ignoring self hit
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	const bool bSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, Location, End,
	                                                           ECollisionChannel::ECC_GameTraceChannel1, Params);
	return bSuccess;
}

AController* AGun::GetOwnerController() const
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}
