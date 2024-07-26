// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class LENA_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 무언가가 구체 컴포넌트에 들어설 때 호출 */
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 
	/** 무언가가 구체 컴포넌트를 나설 때 호출 */
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void AddWidget(UUserWidget* Widget_);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget(UUserWidget* Widget_);

	UFUNCTION(BlueprintImplementableEvent)
	void OutSideEvent();

	UFUNCTION(BlueprintCallable)
	void HideWidgetComponent();

	UFUNCTION(BlueprintCallable)
	void ShowWidgetComponent();

	UPROPERTY(BlueprintReadWrite)
	bool IsDone = false;
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APlayerController *PlayerController;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere)
	USoundBase* SoundEffect;

	UPROPERTY(EditAnywhere, Category="Door")
	TSubclassOf<class UCameraShakeBase> WrongAnswerCameraShakeClass;

	UFUNCTION()  // UFUNCTION() 매크로 추가
	void WrongAnswerDelayFunction();
	
	void SetInstructionWidgetText(FText Text, FLinearColor Color);
	void ClearInstructionWidgetTextDelay(float DelayTime);
};
