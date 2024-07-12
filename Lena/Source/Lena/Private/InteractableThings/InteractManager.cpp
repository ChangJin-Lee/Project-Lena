// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/AInteractManager.h"
#include "EngineUtils.h"

// Sets default values
AAInteractManager::AAInteractManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAInteractManager::BeginPlay()
{
	Super::BeginPlay();

	// 액터 매핑 초기화
	for(TActorIterator<AActor> It(GetWorld()); It; ++It) // #include "EngineUtils.h" 필요
	{
		AActor* Actor = *It;
		if(Actor)
		{
			ActorMap.Add(Actor->GetName(), Actor);
		}
	}

	// 매핑설정
	SetupMappings();
}

// Called every frame
void AAInteractManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AAInteractManager