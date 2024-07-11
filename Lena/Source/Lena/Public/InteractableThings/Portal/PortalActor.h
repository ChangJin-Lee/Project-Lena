// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableThings/InteractableActor.h"
#include "PortalActor.generated.h"

UCLASS()
class LENA_API APortalActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Restart();
};
