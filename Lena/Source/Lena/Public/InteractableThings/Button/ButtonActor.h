// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableThings/InteractableActor.h"
#include "InteractableThings/Door/DoorActor.h"
#include "Items/Base_Item.h"
#include "ButtonActor.generated.h"

UCLASS()
class LENA_API AButtonActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AButtonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Touch")
	TArray<FString> RequiredItem;

	UPROPERTY(EditAnywhere, Category="Touch")
	ADoorActor* DoorActor;

	bool CheckConditions();

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ButtonMesh;
	
	FTimerHandle WrongAnswerDelayHandle;
};
