// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "AdvancedPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API AAdvancedPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AAdvancedPlayerCameraManager();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossesssCameraZoomed();

	UPROPERTY(BlueprintReadOnly)
	int PublicValue = 10;

private:
	APawn* AdvancedControlledPawn;
	
};
