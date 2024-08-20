// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData/InventoryItem.h"
#include "Base_Item.generated.h"

UCLASS()
class LENA_API ABase_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void PickUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> ItemClass; // 아이템 클래스 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UMaterialInterface* ItemImageMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EGunType GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EAmmoType AmmoType;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Captrue")
	USceneCaptureComponent2D* SceneCaptureComponent2D;
};