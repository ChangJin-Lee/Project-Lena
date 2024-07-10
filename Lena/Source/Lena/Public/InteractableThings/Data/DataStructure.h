#pragma once

#include "CoreMinimal.h"
#include "DataStructure.generated.h"

USTRUCT(BlueprintType)
struct FButtonLockButtonData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsButtonMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsCliked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector InitialLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    FButtonLockButtonData()
        : bIsButtonMove(false)
        , IsCliked(false)
        , InitialLocation(FVector::ZeroVector)
        , TargetLocation(FVector::ZeroVector)
    {
    }
};