// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		if (UUserWidget* WinWidget = CreateWidget(this, WinWidgetClass))
		{
			WinWidget->AddToViewport();
		}
	}
	else
	{
		if (UUserWidget* LoseWidget = CreateWidget(this, LoseWidgetClass))
		{
			LoseWidget->AddToViewport();
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
