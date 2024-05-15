// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	// PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// SetFocus(PlayerPawn);
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (LineOfSightTo(PlayerPawn))
	{
		MoveToActor(PlayerPawn,AcceptanceRadius);
		SetFocus(PlayerPawn);
	}
	else
	{
		// 이 우선권들은 레벨이 서로 다른 포커스임
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
	// if lineofsight
	// moveto
	// setfocus
	// else
	// clearfocus
	// stopmovement
}
