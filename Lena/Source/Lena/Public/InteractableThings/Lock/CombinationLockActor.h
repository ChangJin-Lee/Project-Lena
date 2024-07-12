// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Components/TimelineComponent.h"
#include "InteractableThings/Door/DoorActor.h"
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

	virtual void Unlock(AActor* ActorToUnlock) override;

	UFUNCTION(BlueprintCallable)
	void ScrollCombinationLock(FRotator InTargetRotation);

	UFUNCTION()
	void OnTimeLineFinished();

	UFUNCTION(BlueprintCallable)
	void MoveNextWheelMesh();

	UFUNCTION(BlueprintCallable)
	void MovePrevWheelMesh();
	
	UFUNCTION(BlueprintCallable)
	FString GetCurrentDial();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool CheckAnim = false;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* ShackleMesh;

	// WheelMesh Scale3D
	float WheelMeshDefaultScaleSize = 5.0f;
	float WheelMeshSelectedSize = 0;

	UFUNCTION()
	void HandleCombinationLockProgress(float Value);

	UPROPERTY()
	class UTimelineComponent* CombinationLockTimeline;

	UPROPERTY()
	FOnTimelineFloat CombinationLockTimelineCallback;
	
	UPROPERTY()
	FOnTimelineEvent TimelineFinishedCallback;

	UPROPERTY()
	FRotator InitialRotation;

	UPROPERTY()
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CombinationLockCurve;

	bool bIsTimeLinePlaying = false;

	int SelectedWheelIndex = 0;

	TArray<UStaticMeshComponent*> WheelMeshArray;

	// EWheelMeshEnum SelectedWheelMeshEnum; // Selected Mesh Enum

	UStaticMeshComponent* GetSelectedWheelMesh();

	FString GetCurrentDial(UStaticMeshComponent* WheelMeshDial);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class ASlidingDoorActor> DoorActorClass;
	
	ASlidingDoorActor* DoorActor;

	void CheckRightAnswer();

};
