// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorActor.h"
#include "Components/TimelineComponent.h"
#include "HingedDoorActor.generated.h"

/**
 * 
 */

UCLASS()
class LENA_API AHingedDoorActor : public ADoorActor
{
	GENERATED_BODY()

public:
	AHingedDoorActor();

	virtual void Open() override;

	UFUNCTION(BlueprintCallable)
	void OpenHingedDoor(FRotator TargetRotation);

	UFUNCTION(BlueprintCallable)
	void CloseHingedDoor(FRotator TargetRotation);
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleHingedDoorProgress(float Value);

	UPROPERTY()
	UTimelineComponent* HingedDoorTimeline;

	UPROPERTY()
	FOnTimelineFloat HingedDoorTimelineCallback;

	UPROPERTY()
	FRotator InitialRotation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* HingedDoorCurve;
};