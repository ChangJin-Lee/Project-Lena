// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InventorySlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

FReply UInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return Super::OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UDragDropOperation* DragDropOp = NewObject<UDragDropOperation>();
	DragDropOp->DefaultDragVisual = this;
	DragDropOp->Payload = this;
	OutOperation = DragDropOp;
}
