#pragma once

#include <ShlObj.h>

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
	UTexture2D* ItemImage; // 아이템 이미지 추가
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription; // 추가 정보

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item")
	AActor* ItemActor;

	// 추가할 다른 속성들

	FInventoryItem()
		: ItemID(TEXT("")), ItemName(TEXT("")), ItemImage(nullptr), Quantity(1), weight(0), ItemDescription(TEXT("")), ItemActor(nullptr)
	{}
};
