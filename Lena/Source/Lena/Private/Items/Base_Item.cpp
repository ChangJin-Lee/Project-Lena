// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Items/Base_Item.h"
#include "Items/ItemData/ItemData.h"

// Sets default values
ABase_Item::ABase_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Quantity = 1; // 기본 수량 설정
}

// Called when the game starts or when spawned
void ABase_Item::BeginPlay()
{
	Super::BeginPlay();

	InitializeItemProperties();
}

// Called every frame
void ABase_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABase_Item::InitializeItemProperties()
{
	static const FString ContextString(TEXT("ItemDataTableContext"));

	// 데이터 테이블 로드
	UDataTable* ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Data/DataTables/ItemDataTable.ItemDataTable'"));

	if (ItemDataTable)
	{
		// 데이터 테이블에서 ItemID에 해당하는 행을 찾습니다.
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(*ItemID, ContextString);
		if (ItemData)
		{
			// 아이템 속성을 설정합니다.
			ItemName = ItemData->ItemName;
			Quantity = ItemData->Quantity;
			ItemDescription = ItemData->ItemDescription;
		}
	}
}