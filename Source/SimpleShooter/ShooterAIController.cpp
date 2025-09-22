// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBehavior)
	{
		return;
	}
	
	RunBehaviorTree(AIBehavior);

	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!PlayerPawn)
	{
		return;
	}
	
	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), PlayerPawn->GetActorLocation());
}
