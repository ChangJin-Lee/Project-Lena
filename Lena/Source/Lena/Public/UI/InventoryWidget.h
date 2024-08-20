// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Base_Character.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
/**
 * 
 */
UCLASS()
class LENA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
public:
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D InventoryAreaMin = FVector2D(430,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D InventoryAreaMax = FVector2D(730,1030);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D GroundAreaMin = FVector2D(80,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D GroundAreaMax = FVector2D(380,1030);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D WeaponAreaMin = FVector2D(1380,50);
	UPROPERTY(EditAnywhere, Category="Drag And Drop")
	FVector2D WeaponAreaMax = FVector2D(1880,1030);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveGroundSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void AddGroundSlot();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BuildInventorySlot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BuildGroundSlot();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BuildEquipmentSlot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABase_Character* Base_Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AShooterPlayerController* PlayerController;
	
private:
	// InventoryArea
	bool IsInInventoryArea(FVector2D LocalPosition);
	void SetInventorySlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);

	//GroundArea
	bool IsInGroundArea(FVector2D LocalPosition);
	void SetGroundSlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);

	// WeaponSlot
	bool IsInWeaponArea(FVector2D LocalPosition);
	void SetEquipSlot(int32 SlotIndex, UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType);
	int32 GetEquipSlotIndex(FVector2D LocalPosition);
};
