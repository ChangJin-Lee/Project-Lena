// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorActor.h"
#include "Components/TimelineComponent.h"
#include "SlidingDoorActor.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API ASlidingDoorActor : public ADoorActor
{
	GENERATED_BODY()

public:
	ASlidingDoorActor();

	UFUNCTION(BlueprintCallable)
	void OpenSlidingDoor(FVector TargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void RightAnswer(FVector TargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void WrongAnswer();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleSlidingDoorProgress(float Value);

	UPROPERTY()
	class UTimelineComponent* SlidingDoorTimeline;

	UPROPERTY()
	FOnTimelineFloat SlidingDoorTimelineCallback;

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* SlidingDoorCurve;
	
	FTimerHandle WrongAnswerDelayHandle;
	
	UFUNCTION()  // UFUNCTION() 매크로 추가
	void WrongAnswerDelayFunction();

	void StartWrongAnswerDelay(float DelayTime);
};