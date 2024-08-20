// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemSpawnManager.h"

#include "Items/Base_Item.h"
#include "Items/ItemData/ItemSpawnData.h"


// Sets default values
AItemSpawnManager::AItemSpawnManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(!ItemSpawnTable)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable>  ItemSpawnDataObject(TEXT("DataTable'/Game/Data/DataTables/ItemSpawnData.ItemSpawnData'"));
		if(ItemSpawnDataObject.Succeeded())
		{
			ItemSpawnTable = ItemSpawnDataObject.Object;
		}
	}
}

// Called when the game starts or when spawned
void AItemSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnItemsFromTable();
	
}

// Called every frame
void AItemSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AItemSpawnManager::SpawnItemsFromTable()
{
	if(!ItemSpawnTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSpawnTable is not valid!"));
		return;
	}

	const FString ContextString(TEXT("Item Spawn Data Context"));
	TArray<FItemSpawnData*> AllRows;
	ItemSpawnTable->GetAllRows(ContextString, AllRows);

	for(FItemSpawnData* Row : AllRows)
	{
		if(Row && Row->ItemClass && Row->SpawnPointActor)
		{
			FTransform SpawnTransform = Row->SpawnPointActor->GetActorTransform();
			AActor* SpawneddItemActor = GetWorld()->SpawnActor<AActor>(Row->ItemClass, SpawnTransform);
			if(SpawneddItemActor)
			{
				ABase_Item* SpawnedItem = Cast<ABase_Item>(SpawneddItemActor);
				if(SpawnedItem)
				{
					SpawnedItem->ItemID = Row->ItemID;
					SpawnedItem->ItemName = Row->ItemName;
					SpawnedItem->ItemClass = Row->ItemClass;
					SpawnedItem->ItemImage = Row->ItemImage;
					SpawnedItem->Weight = Row->Weight;
					SpawnedItem->ItemDescription = Row->ItemDescription;
					SpawnedItem->Quantity = Row->Quantity;
					SpawnedItem->EquipmentType = Row->EquipmentType;
					SpawnedItem->GunType = Row->GunType;
					SpawnedItem->ItemType = Row->ItemType;
					SpawnedItem->AmmoType = Row->AmmoType;
				}
			}
		}
	}
}

