// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

UCLASS()
class LENA_API AInventory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FItemData> Items;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(const FItemData& ItemData);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(const FString& ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UpdateItemQuantity(const FString& ItemID, int32 NewQuantity);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 FindItemByID(const FString& ItemID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
