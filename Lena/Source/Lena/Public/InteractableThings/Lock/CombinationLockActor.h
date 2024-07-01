// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Components/TimelineComponent.h"
#include "CombinationLockActor.generated.h"

UCLASS()
class LENA_API ACombinationLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACombinationLockActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ScrollCombinationLock(FRotator InTargetRotation);

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* WheelSceneComponent;
		
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelMesh;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelMesh1;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelMesh2;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WheelMesh3;

	UFUNCTION()
	void HandleCombinationLockProgress(float Value);

	UPROPERTY()
	class UTimelineComponent* CombinationLockTimeline;

	UPROPERTY()
	FOnTimelineFloat CombinationLockTimelineCallback;

	UPROPERTY()
	FRotator InitialRotation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CombinationLockCurve;
};
