// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Game/GameMode/KillEmAllGameMode.h"

#include "EngineUtils.h"
#include "Lena/Public/Game/Controller/ShooterAIController.h"
#include "GameFramework/Controller.h"

class ALoseController;

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	UE_LOG(LogTemp, Warning, TEXT("A Pawn was Killed!"));
	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if(PlayerController != nullptr)
	{
		EndGame(false);
	}

	for ( AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
	{
		if(!Controller->IsDead())
		{
			return;
		}
	}
	// 플레이어가 승리
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for ( AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
