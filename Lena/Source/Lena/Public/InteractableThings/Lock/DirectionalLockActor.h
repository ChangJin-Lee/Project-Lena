// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LockActor.h"
#include "Components/TimelineComponent.h"
#include "DirectionalLockActor.generated.h"

UENUM(BlueprintType)
enum class DirectionEnum : uint8
{
	None UMETA(DisplayName = "None"),
	Right UMETA(DisplayName = "→"),
	Left UMETA(DisplayName = "←"),
	Up UMETA(DisplayName = "↑"),
	Down UMETA(DisplayName = "↓")
};

UCLASS()
class LENA_API ADirectionalLockActor : public ALockActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADirectionalLockActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void MoveFromStart(FVector InputVector, FString direction);

	UFUNCTION(BlueprintCallable)
	bool IsMovementProgressing();

	void CheckRightAnswer();
	
	FString EnumToString(DirectionEnum EnumValue);
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DirectionalLockSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DirectionalLockBodyMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DirectionalLockshackleMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DirectionalLockBallMeshComponent;

	// TimeLine Movement
	
	UPROPERTY()
	UTimelineComponent* DirectionalLockTimeLine;

	UPROPERTY()
	FOnTimelineFloat DirectionalLockTimeLineCallback;

	UPROPERTY()
	FOnTimelineEvent DirectionalLockTimeLineFinishedCallback;

	UFUNCTION()
	void HandleDirectionalLockProgress(float value);

	UFUNCTION()
	void HandleDirectionalLockFinished();

	UPROPERTY(EditAnywhere, Category="TimeLine")
	UCurveFloat* DirectionalLockCurve;

	bool bIsTimeLinePlaying = false;

	UPROPERTY(BlueprintReadWrite, Category = "TimeLine", meta = (AllowPrivateAccess="true"))
	bool bIsMovementProgressing = false;

	UPROPERTY()
	FVector InitialLocation;
	
	UPROPERTY()
	FVector TargetLocation;


	// Open Door
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Open Door", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class ASlidingDoorActor> DoorActorClass;

	ASlidingDoorActor* DoorActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Open Door", meta=(AllowPrivateAccess="true"))
	FString InputPassWord;

	UPROPERTY(EditAnywhere, Category="Door")
	TSubclassOf<class UCameraShakeBase> WrongAnswerCameraShakeClass;

	FString WidgetDisplayPassword = "";
	
};
