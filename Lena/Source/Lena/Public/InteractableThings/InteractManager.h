// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ConditionMapping.h"
#include "GameFramework/Actor.h"
#include "InteractManager.generated.h"

UCLASS()
class LENA_API AInteractManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LockManager")
	UDataTable* LockDoorMappingTable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetupLockAndDoor();
	void SetupConditionWithActor(AActor* Actor, const FConditionEntry& ConditionEntry);

	TMap<FString, AActor*> LockMap;
	TMap<FString, AActor*> DoorMap;
	TMap<FString, AActor*> ItemMap;

	void LoadDataTableForLevel();
};
