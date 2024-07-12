// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ConditionMapping.h"
#include "GameFramework/Actor.h"
#include "AInteractManager.generated.h"

UCLASS()
class LENA_API AAInteractManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAInteractManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LockManager")
	UDataTable* MappingTable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetupMappings();
	void SetupConditionWithActor(AActor* Actor, const FConditionEntry& ConditionEntry);

	TMap<FString, AActor*> ActorMap;
	TMap<FString, bool> ItemMap;
	TMap<FString, FString> DialogueMap;
	TMap<FString, bool> LocationMap;
};
