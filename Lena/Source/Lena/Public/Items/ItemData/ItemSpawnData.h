#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Engine/DataTable.h"
#include "ItemSpawnData.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Data")
	TSoftObjectPtr<AActor> SpawnPointActor;  // TSoftObjectPtr 사용
	// TSubclassOf<AActor> SpawnPointActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemImage; // 아이템 이미지 추가
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UMaterialInterface* ItemImageMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription; // 특정 문을 열기 위한 열쇠 등의 조건을 나타내는 값
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float DefensePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EGunType GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EAmmoType AmmoType;
};