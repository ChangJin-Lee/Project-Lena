// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PasswordWidget;

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
	void AddWidget();

	UFUNCTION(BlueprintCallable)
	void RemoveWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OutSideEvent();

	UFUNCTION(BlueprintCallable)
	void HideTextRenderComponent();

	UFUNCTION(BlueprintCallable)
	void ShowTextRenderComponent();

	UPROPERTY(BlueprintReadWrite)
	bool IsDone = false;
	
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* HitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APlayerController *PlayerController;

	UUserWidget* Widget;
};
