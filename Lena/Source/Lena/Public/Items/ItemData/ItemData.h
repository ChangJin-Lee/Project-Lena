#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription; // 특정 문을 열기 위한 열쇠 등의 조건을 나타내는 값
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float DefensePower;
};