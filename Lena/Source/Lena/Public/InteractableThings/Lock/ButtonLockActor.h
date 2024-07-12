// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Data/DataStructure.h"
#include "ButtonLockActor.generated.h"

UCLASS()

class LENA_API AButtonLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AButtonLockActor();
	
	UFUNCTION(BlueprintCallable)
	void MoveButton(UStaticMeshComponent* TargetMeshComponent);

	UFUNCTION(BlueprintCallable)
	void ZoomInCamera();

	UFUNCTION(BlueprintCallable)
	void ZoomOutCamera();

	UFUNCTION(BlueprintCallable)
	bool ValidPassword(FString input);
	
	UFUNCTION(BlueprintCallable)
	void OpenLock();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Button Lock
	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	USceneComponent* ButtonLockSceneComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	UStaticMeshComponent* ButtonLockBodyMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	UStaticMeshComponent* ButtonLockKeypadMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	UStaticMeshComponent* ButtonLockShackleMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<UStaticMeshComponent*> ButtonMeshComponents;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<UTimelineComponent*> ButtonTimelines;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<FButtonLockButtonData> ButtonDataArray;

	int32 ButtonNums = 8;

	UPROPERTY(EditAnywhere, Category="TimeLine")
	UCurveFloat* ButtonLockCurve;
	
	UFUNCTION()
	void HandleButtonLockProgress(float value, int32 ButtonIndex);
	
	UFUNCTION()
	void HandleButtonLockFinished();

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FVector TargetLocation;

	int32 SelectedButtonIndex = 0;

	void ButtonMovePlayFromStart(int32 index, float value);
	void DestroyButtonLock();
	
	// Camera
	UPROPERTY(EditAnywhere, Category="Camera")
	UTimelineComponent* CameraMoveTimelineComponent;

	UPROPERTY(EditAnywhere, Category="Camera")
	UCurveFloat* CameraMoveCurve;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	FOnTimelineFloat CameraMoveCallback;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	FOnTimelineEvent CameraMoveFinishedCallback;

	APlayerCameraManager* PlayerCameraManager;

	APlayerController* PlayerController;

	ACharacter* PlayerCharacter;

	FVector InitialCameraLocation;
	FRotator InitialCameraRotation;

	FVector ZoomedCameraLocation;
	FRotator ZoomedCameraRotation;

	void LockCamera();
	void UnLockCameara();

	void MoveFinished();
	void MoveCamera(AActor* TargetActor);
	void OnMouseClick();

	bool bIsZoomedIn = false;
	
	AActor* CameraTargetLocationActor;
	AActor* CameraInitialLocationActor;

	AActor* CreateTargetActor(FVector Location, FRotator Rotation);
};
