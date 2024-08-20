// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryHoverWidget.h"
#include "InventoryWidget.h"
#include "../Characters/Base_Character.h"
#include "../Items/ItemData/InventoryItem.h"

#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ref")
	FInventoryItem ItemDataRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ref")
	ABase_Character* Base_CharacterRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ref")
	UInventoryWidget* InventoryMainRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ref")
	UInventoryHoverWidget* InventoryHoverWidgetRef;
};
