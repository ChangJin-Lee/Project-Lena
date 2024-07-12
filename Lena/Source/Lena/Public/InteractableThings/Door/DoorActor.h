// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableThings/InteractableActor.h"
#include "DoorActor.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class LENA_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ADoorActor();

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OpenDoorEvent();

	UFUNCTION(BlueprintCallable)
	bool CheckRequiredItem();
	
	FString GetPassWord();

	UFUNCTION(BlueprintCallable, Category="Door")
	virtual void Open();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NumpadWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	FString PassWord = "";

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* FrameMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	FName RequiredItem;
	
	UPROPERTY(EditAnywhere, Category="Door")
	TSubclassOf<class UCameraShakeBase> WrongAnswerCameraShakeClass;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* DoorOpenSound;
	
};
