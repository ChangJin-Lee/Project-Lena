// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	// PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	// SetFocus(PlayerPawn);

	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation() );
		
		UE_LOG(LogTemp, Warning, TEXT("Health Left : %s"), *GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation")).ToString());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (LineOfSightTo(PlayerPawn))
	{
		// Setting PlayerLocation
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		// Setting LastKnown
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());

	}
	else
	{
		// Clear PlayerLocation
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		UE_LOG(LogTemp, Warning, TEXT("Chase LastKnownLocation : %s"), *GetBlackboardComponent()->GetValueAsVector(TEXT("LastKnownLocation")).ToString());
	}
}
