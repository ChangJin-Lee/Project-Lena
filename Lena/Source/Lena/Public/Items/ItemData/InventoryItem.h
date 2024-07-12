#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription; // 추가 정보

	// 추가할 다른 속성들

	FInventoryItem()
		: ItemID(TEXT("")), ItemName(TEXT("")), Quantity(1), ItemDescription(TEXT(""))
	{}
};
