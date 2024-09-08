// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemData/InventoryItem.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LENA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FInventoryItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float CurrentWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	float MaxWeight;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(const FInventoryItem& ItemData);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(const FInventoryItem& ItemData);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool ResetItem(const FInventoryItem& ItemData);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItemByName(const FString& ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UpdateItemQuantity(const FString& ItemID, int32 NewQuantity);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 FindItemByID(const FString& ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 FindItemByDescription(const FString& ItemDescription);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void ClearItems(); // 추가된 함수

};
