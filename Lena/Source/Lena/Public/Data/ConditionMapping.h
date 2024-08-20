#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConditionMapping.generated.h"

USTRUCT(BlueprintType)
struct FConditionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ConditionType; // "Lock", "Item", "Dialogue", "Location", etc

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ConditionID; // LockID, ItemID, NPCID, LocionID, etc

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> AdditionalData; // PassWord, DialogueLine, Location, etc
};

USTRUCT(BlueprintType)
struct FActorEntry : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConditionEntry> RequiredConditions;
};