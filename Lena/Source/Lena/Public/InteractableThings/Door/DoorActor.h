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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Lock")
	FString RequiredItemDescription;
	
	FString GetPassWord();

	UFUNCTION(BlueprintCallable, Category="Door")
	virtual void Open();

	UFUNCTION(BlueprintCallable, Category="Door")
	virtual void Close();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrameMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NumpadWidgetComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	FString PassWord = "";
	
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* DoorOpenSound;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* DoorCloseSound;
	
	UPROPERTY(EditAnywhere, Category="Door")
	TSubclassOf<class UCameraShakeBase> WrongAnswerCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta = (AllowPrivateAccess = "true"))
	FName RequiredItem;

};
