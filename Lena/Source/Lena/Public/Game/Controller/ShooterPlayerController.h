// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Base_Character.h"
#include "GameFramework/PlayerController.h"
#include "UI/InteractWidget.h"
#include "UI/InventoryWidget.h"
#include "ShooterPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class LENA_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	void EnableMouseClick();
	void DisableMouseClick();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* IA_MouseClick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Input")
	UInputAction* IA_PickUpItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* IMC_Interaction;

	void HandleMouseClick();
	
	//Widget
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	UInteractWidget* ItemInfoWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"), Category="Inventory")
	bool IsInWidget = false;

	ABase_Character* Base_Character;
	ABase_Item* LineTraceItem;

	FTimerHandle TimerHandle_CheckPickUpItem;
	
public:
	UFUNCTION(BlueprintCallable)
	void InputModeUI();
	UFUNCTION(BlueprintCallable)
	void InputModeGame();
	
	void HandlePickUpItem();
	void CheckPickUpItemLine();
	UFUNCTION(BlueprintCallable)
	void CheckPickUpItemSweep();
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	UPROPERTY()
	UUserWidget* HUD;
	
	FTimerHandle RestartTimer;

	void OnMouseClick();
	bool bIsClickEnabled;
};
