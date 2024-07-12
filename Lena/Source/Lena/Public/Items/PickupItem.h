// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Components/PickupableItemComponent.h"
#include "Lena/Public/InteractableThings/InteractableActor.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"



class UBoxComponent;

UCLASS()
class LENA_API APickupItem : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UPickupableItemComponent* PickupableItemComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickUp(AActor* InteractingActor);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;
};
