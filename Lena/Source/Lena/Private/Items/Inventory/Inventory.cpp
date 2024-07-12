// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/Inventory.h"
#include "Engine/DataTable.h"

// Sets default values
AInventory::AInventory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AInventory::AddItem(const FInventoryItem& ItemData)
{
	int32 ItemIndex = FindItemByID(ItemData.ItemID);
	if (ItemIndex != INDEX_NONE)
	{
		Items[ItemIndex].Quantity += ItemData.Quantity;
	}
	else
	{
		Items.Add(ItemData);
	}
}

bool AInventory::RemoveItem(const FString& ItemID)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemID)
		{
			Items.RemoveAt(i);
			return true;
		}
	}
	return false;
}

bool AInventory::UpdateItemQuantity(const FString& ItemID, int32 NewQuantity)
{
	
	for (FInventoryItem& Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			Item.Quantity = NewQuantity;
			return true;
		}
	}
	return false;
}

int32 AInventory::FindItemByID(const FString& ItemID)
{
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		if (Items[Index].ItemID == ItemID)
		{
			return Index;
		}
	}
	return INDEX_NONE;
}

int32 AInventory::FindItemByDescription(const FString& ItemDescription)
{
	for (int32 Index = 0; Index < Items.Num(); ++Index)
	{
		if (Items[Index].ItemDescription == ItemDescription)
		{
			return Index;
		}
	}
	return INDEX_NONE;
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

