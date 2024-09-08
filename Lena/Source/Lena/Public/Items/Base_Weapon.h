// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Item.h"
#include "Base_Weapon.generated.h"

UCLASS()
class LENA_API ABase_Weapon : public ABase_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
