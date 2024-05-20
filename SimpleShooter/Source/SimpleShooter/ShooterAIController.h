// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController 
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsDead() const;

private:
	APawn* PlayerPawn;

	// UPROPERTY(EditAnywhere)
	// float AcceptanceRadius = 150;
	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
