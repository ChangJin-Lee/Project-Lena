// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ShooterCharacter.h"
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
		
		// UE_LOG(LogTemp, Warning, TEXT("Health Left : %s"), *GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation")).ToString());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
	 AShooterCharacter* ControlledCharactor = Cast<AShooterCharacter>(GetPawn());
	if(ControlledCharactor != nullptr)
	{
		return ControlledCharactor->IsDead();
	}

	return true;
}

