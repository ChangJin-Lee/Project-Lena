// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "DirectionalLockActor.generated.h"

UCLASS()
class LENA_API ADirectionalLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADirectionalLockActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
