// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ALenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn *PawnKilled);
};
