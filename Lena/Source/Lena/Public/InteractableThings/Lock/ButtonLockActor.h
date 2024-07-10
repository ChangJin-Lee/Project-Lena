// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "ButtonLockActor.generated.h"

UCLASS()
class LENA_API AButtonLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AButtonLockActor();

	int SelectedButtonIndex = 0;
	
	UFUNCTION(BlueprintCallable)
	void MoveButton(UStaticMeshComponent* TargetMeshComponent);

	UFUNCTION(BlueprintCallable)
	void ZoomInCamera();

	UFUNCTION(BlueprintCallable)
	void ZoomOutCamera();
	
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
	TArray<UStaticMeshComponent*> ButtonLockButtonMeshComponents;

	UPROPERTY(VisibleAnywhere, Category="ButtonLock")
	TArray<bool> ClikedButtons;
	
	UPROPERTY(EditAnywhere, Category="TimeLine")
	UTimelineComponent* ButtonLockTimelineComponent;

	UPROPERTY(EditAnywhere, Category="TimeLine")
	UCurveFloat* ButtonLockCurve;

	UPROPERTY(EditAnywhere, Category="TimeLine")
	FOnTimelineFloat ButtonLockButtonCallback;

	UFUNCTION()
	void HandleButtonLockProgress(float value);

	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY()
	FVector TargetLocation;

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

	void HandleCameraMoveProgress(float value);
	void HandleCameraMoveFinished();

	FVector InitialCameraLocation;
	FRotator InitialCameraRotation;

	FVector ZoomedCameraLocation;
	FRotator ZoomedCameraRotation;

	void LockCamera();
	void UnLockCameara();

	bool bIsZoomedIn = false;
	
	AActor* CameraTargetLocationActor;
	AActor* CameraInitialLocationActor;

	AActor* CreateTargetActor(FVector Location, FRotator Rotation);
};
