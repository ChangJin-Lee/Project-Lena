// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/InteractManager.h"
#include "EngineUtils.h"
#include "InteractableThings/Door/DoorActor.h"
#include "InteractableThings/Lock/LockActor.h"
#include "Engine/World.h"
#include "Interface/InteractActionInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractManager::AInteractManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractManager::BeginPlay()
{
	Super::BeginPlay();

	// 특정 레벨에 대한 DataTable을 로드
	FString LevelName = GetWorld()->GetMapName();
	LoadDataTableForLevel(LevelName);


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

void AInteractManager::LoadDataTableForLevel(const FString& LevelName)
{
	FString DataTablePath = FString::Printf(TEXT("DataTable'/Game/Data/DataTables/%s_InteractConditionTable.%s_InteractConditionTable'"), *LevelName, *LevelName);
	static ConstructorHelpers::FObjectFinder<UDataTable> LockDoorMappingData(*DataTablePath);

	if (LockDoorMappingData.Succeeded())
	{
		LockDoorMappingTable = LockDoorMappingData.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load DataTable for level: %s"), *LevelName);
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
	}
	
	const FString ContextString(TEXT("LockDoorMappingTableContext"));
	TArray<FActorEntry*> AllRows;
	LockDoorMappingTable->GetAllRows(ContextString, AllRows);

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
		AActor** FoundLock = LockMap.Find(ConditionEntry.ConditionID);
		if(FoundLock)
		{
			ALockActor* LockActor = Cast<ALockActor>(*FoundLock);
			ADoorActor* DoorActor = Cast<ADoorActor>(Actor);
			if(LockActor)
			{
				// 특정 자물쇠와 문을 연결하는 로직 추가
				// 예를 들어, 자물쇠가 풀리면 문을 여는 로직
				LockActor->Password = ConditionEntry.AdditionalData;
				LockActor->TargetDoor = DoorActor;
			}
		}
	}
	else if (ConditionEntry.ConditionType == "Item")
	{
		
	}
	else if (ConditionEntry.ConditionType == "Dialogue")
	{
	
	}
	else if (ConditionEntry.ConditionType == "Location")
	{
		
	}
}
