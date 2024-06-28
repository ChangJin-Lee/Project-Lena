// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "DoorActor.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ADoorActor();

	UFUNCTION(BlueprintCallable)
	void OpenSlidingDoor(FVector Location);

	UFUNCTION(BlueprintCallable)
	void OpenHingedDoor(FRotator Rotation);

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OpenDoorEvent();

	UFUNCTION(BlueprintCallable)
	bool CheckRequiredItem();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* FrameMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NumpadWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	FName RequiredItem;
	
	UPROPERTY(EditAnywhere, Category="Door")
	TSubclassOf<class UCameraShakeBase> WrongAnswerCameraShakeClass;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* DoorOpenSound;
};
