// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"

class ALoseController;

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	UE_LOG(LogTemp, Warning, TEXT("A Pawn was Killed!"));
	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if(PlayerController != nullptr)
	{
		PlayerController->GameHasEnded(nullptr, false);
	}
}
