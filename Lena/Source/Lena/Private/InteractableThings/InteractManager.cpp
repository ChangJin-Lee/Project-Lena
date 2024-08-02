// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/InteractManager.h"
#include "EngineUtils.h"
#include "InteractableThings/Door/DoorActor.h"
#include "InteractableThings/Lock/LockActor.h"
#include "Engine/World.h"
#include "InteractableThings/Button/ButtonActor.h"
#include "Interface/InteractActionInterface.h"
#include "Items/Base_Item.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractManager::AInteractManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LoadDataTableForLevel();
}

// Called when the game starts or when spawned
void AInteractManager::BeginPlay()
{
	Super::BeginPlay();

	// Set a timer to call SetupLocksAndDoors on the next tick
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AInteractManager::SetupLockAndDoor);
	// 매핑설정
	// SetupLockAndDoor();
}

// Called every frame
void AInteractManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractManager::LoadDataTableForLevel()
{
	FString DataTablePath = FString::Printf(TEXT("DataTable'/Game/Data/DataTables/InteractConditionTable.InteractConditionTable'"));
	static ConstructorHelpers::FObjectFinder<UDataTable> LockDoorMappingData(*DataTablePath);

	if (LockDoorMappingData.Succeeded())
	{
		LockDoorMappingTable = LockDoorMappingData.Object;
	}
}


void AInteractManager::SetupLockAndDoor()
{
	// 액터 매핑 초기화
	for(TActorIterator<AActor> It(GetWorld()); It; ++It) // #include "EngineUtils.h" 필요
	{
		AActor* Actor = *It;
		if(Actor->IsA<ALockActor>())
		{
			LockMap.Add(Actor->GetActorLabel(), Actor);
		}
		else if (Actor->IsA<ADoorActor>())
		{
			DoorMap.Add(Actor->GetActorLabel(), Actor);
		}
		else if (Actor->IsA<ABase_Item>())
		{
			ABase_Item* Item = Cast<ABase_Item>(Actor);
			if(ItemMap.Contains(Item->ItemName))
			{
				ItemMap[Item->ItemName].Add(Item);
			}
			else
			{
				TArray<AActor*> NewArray;
				NewArray.Add(Item);
				ItemMap.Add(Item->ItemName, NewArray);
			}
		}
		else if (Actor->IsA<AButtonActor>())
		{
			AButtonActor* Button = Cast<AButtonActor>(Actor);
			ButtonMap.Add(Button->GetActorLabel(), Button);
		}
	}
	
	const FString ContextString(TEXT("LockDoorMappingTableContext"));
	TArray<FActorEntry*> AllRows;
	LockDoorMappingTable->GetAllRows(ContextString, AllRows);

	for(auto k : ButtonMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("ButtonMap :  %s"), *k.Key);
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *k.Value->GetActorLabel());
	}

	// Door 기준
	for(FActorEntry* Row : AllRows)
	{
		if(Row)
		{
			AActor** FoundActor = DoorMap.Find(Row->ActorID);
			if(FoundActor)
			{
				for(const FConditionEntry& Condition : Row->RequiredConditions)
				{
					SetupConditionWithActor(*FoundActor, Condition);
				}
			}
		}
	}
}

void AInteractManager::SetupConditionWithActor(AActor* Actor, const FConditionEntry& ConditionEntry)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupConditionWithActor start"));
	if(ConditionEntry.ConditionType == "Lock")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Locked";
		
		for (int32 i = 0; i < ConditionEntry.ConditionID.Num(); ++i)
		{
			const FString& LockID = ConditionEntry.ConditionID[i];
			AActor** FoundLock = LockMap.Find(LockID);
			if (FoundLock)
			{
				ALockActor* LockActor = Cast<ALockActor>(*FoundLock);
				if (LockActor)
				{
					// 같은 인덱스의 AdditionalData 값을 사용
					if (ConditionEntry.AdditionalData.IsValidIndex(i))
					{
						LockActor->Password = ConditionEntry.AdditionalData[i];
					}
					LockActor->TargetDoor = DoorActor;
				}
			}
		}
	}
	else if (ConditionEntry.ConditionType == "Item")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Item";
		for(const FString& ItemID : ConditionEntry.ConditionID)
		{
			DoorActor->RequiredItem.Add(ItemID);
		}
	}
	else if (ConditionEntry.ConditionType == "Default")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Default";
	}
	else if (ConditionEntry.ConditionType == "Button")
	{
		ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
		DoorActor->RequiredCondition = "Button";
		for(int32 i = 0 ; i <  ConditionEntry.ConditionID.Num(); ++i)
		{
			const FString ButtonName = ConditionEntry.ConditionID[i];
			AActor** FindActor = ButtonMap.Find(ButtonName);
			if(FindActor)
			{
				AButtonActor* ButtonActor = Cast<AButtonActor>(*FindActor);
				ButtonActor->DoorActor = DoorActor;
				if(!ConditionEntry.AdditionalData.IsEmpty())
				{
					ButtonActor->RequiredItem.Add(ConditionEntry.AdditionalData[i]);
				}
			}
		}
	}
	else if (ConditionEntry.ConditionType == "Dialogue")
	{
	
	}
	else if (ConditionEntry.ConditionType == "Location")
	{
		
	}
}
