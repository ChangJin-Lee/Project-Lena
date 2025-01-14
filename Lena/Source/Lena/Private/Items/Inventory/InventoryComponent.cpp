// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/InventoryComponent.h"
#include "Engine/DataTable.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	for(FInventoryItem Item : Items)
	{
		CurrentWeight += Item.Quantity * Item.Weight;
	}

	MaxWeight = 20;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Items.Empty();
}


void UInventoryComponent::AddItem(const FInventoryItem& ItemData)
{
	int32 ItemIndex = FindItemByID(ItemData.ItemID);
	if (ItemIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Items[ItemIndex].Quantity : %d"), Items[ItemIndex].Quantity);
		Items[ItemIndex].Quantity += ItemData.Quantity;
	}
	else
	{
		Items.Add(ItemData);
	}

	CurrentWeight += ItemData.Quantity * ItemData.Weight;
}

bool UInventoryComponent::RemoveItem(const FInventoryItem& ItemData)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemData.ItemID)
		{
			CurrentWeight -= Items[i].Quantity * Items[i].Weight;
			UE_LOG(LogTemp, Warning, TEXT("Remove %s "), *Items[i].ItemName);
			Items.RemoveAt(i);
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::ResetItem(const FInventoryItem& ItemData)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemID == ItemData.ItemID)
		{
			CurrentWeight -= Items[i].Quantity * Items[i].Weight;
			Items[i].Empty();
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::RemoveItemByName(const FString& ItemName)
{
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ItemName == ItemName)
		{
			Items.RemoveAt(i);
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::UpdateItemQuantity(const FString& ItemID, int32 NewQuantity)
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

int32 UInventoryComponent::FindItemByID(const FString& ItemID)
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

int32 UInventoryComponent::FindItemByDescription(const FString& ItemDescription)
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

void UInventoryComponent::ClearItems() 
{
	for (FInventoryItem& Item : Items)
	{
		if (Item.ItemActor)
		{
			Item.ItemActor = nullptr;
		}
	}
	Items.Empty();
}