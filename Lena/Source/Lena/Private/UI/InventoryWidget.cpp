// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InventoryWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Characters/Base_Character.h"
#include "Game/Controller/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InventoryEquipmentSlotWidget.h"
#include "UI/InventoryGroundSlotWidget.h"
#include "UI/InventoryItemSlotWidget.h"
#include "UI/InventorySlotWidget.h"

void UInventoryWidget::NativeConstruct()
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if(Character)
	{
		Base_Character = Cast<ABase_Character>(Character);
	}

	AController* PC = Base_Character->GetController();
	if(PC)
	{
		PlayerController = Cast<AShooterPlayerController>(PC);
	}
	
	BuildInventorySlot();
	BuildGroundSlot();
	BuildEquipmentSlot();
}


bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	FVector2D DropPosition = InDragDropEvent.GetScreenSpacePosition();
	FVector2D LocalPosition = InGeometry.AbsoluteToLocal(DropPosition);

	if(InOperation->Payload)
	{
		UE_LOG(LogTemp, Warning, TEXT("InOperation! %s"), *InOperation->Payload->GetClass()->GetName());

		UInventorySlotWidget* SlotWidget = Cast<UInventorySlotWidget>(InOperation->Payload);

		if(SlotWidget)
		{
			if(IsInGroundArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryItemSlotWidget>())
				{
					SetGroundSlot(SlotWidget, EInventorySlotType::ISt_Inventory);
				}
				else if(SlotWidget->IsA<UInventoryEquipmentSlotWidget>())
				{
					SetGroundSlot(SlotWidget, EInventorySlotType::ISt_Equipment);
				}
			}
			else if(IsInInventoryArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryGroundSlotWidget>())
				{
					SetInventorySlot(SlotWidget, EInventorySlotType::ISt_Ground);
				}
				else if(SlotWidget->IsA<UInventoryEquipmentSlotWidget>())
				{
					SetInventorySlot(SlotWidget, EInventorySlotType::ISt_Equipment);
				}
			}
			else if (IsInWeaponArea(LocalPosition))
			{
				if(SlotWidget->IsA<UInventoryItemSlotWidget>())
				{
					SetEquipSlot(GetEquipSlotIndex(LocalPosition),SlotWidget, EInventorySlotType::ISt_Inventory);
				}
				else if(SlotWidget->IsA<UInventoryGroundSlotWidget>())
				{
					SetEquipSlot(GetEquipSlotIndex(LocalPosition),SlotWidget, EInventorySlotType::ISt_Ground);
				}
			}

			BuildGroundSlot();
			BuildInventorySlot();
			BuildEquipmentSlot();
			if(SlotWidget->InventoryHoverWidgetRef)
			{
				SlotWidget->InventoryHoverWidgetRef->RemoveFromParent();
			}
		}
	}
	return false;
}


bool UInventoryWidget::IsInGroundArea(FVector2D LocalPosition)
{
	return LocalPosition.X > GroundAreaMin.X && LocalPosition.X < GroundAreaMax.X && LocalPosition.Y > GroundAreaMin.Y && LocalPosition.Y < GroundAreaMax.Y;
}

bool UInventoryWidget::IsInInventoryArea(FVector2D LocalPosition)
{
	return LocalPosition.X > InventoryAreaMin.X && LocalPosition.X < InventoryAreaMax.X && LocalPosition.Y > InventoryAreaMin.Y && LocalPosition.Y < InventoryAreaMax.Y;
}

bool UInventoryWidget::IsInWeaponArea(FVector2D LocalPosition)
{
	
	return LocalPosition.X > WeaponAreaMin.X && LocalPosition.X < WeaponAreaMax.X && LocalPosition.Y > WeaponAreaMin.Y && LocalPosition.Y < WeaponAreaMax.Y;
}

void UInventoryWidget::SetGroundSlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->ItemDataRef;
	SlotWidget->Base_CharacterRef->DropItem(ItemData, SlotType);
				
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}


void UInventoryWidget::SetInventorySlot(UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->ItemDataRef;
	SlotWidget->Base_CharacterRef->PickupItem(ItemData.ItemActor, SlotType);
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}

void UInventoryWidget::SetEquipSlot(int32 SlotIndex, UInventorySlotWidget* SlotWidget, EInventorySlotType SlotType)
{
	FInventoryItem ItemData = SlotWidget->ItemDataRef;
	SlotWidget->Base_CharacterRef->EquipItem(ItemData, SlotType, SlotIndex);

	UE_LOG(LogTemp, Warning, TEXT("SlotIndex : %d "), SlotIndex);
	if(PlayerController)
	{
		PlayerController->CheckPickUpItemSweep();
	}
}

int32 UInventoryWidget::GetEquipSlotIndex(FVector2D LocalPosition)
{
	const int32 NumSlots = 4;
	const float SlotHeight = (WeaponAreaMax.Y - WeaponAreaMin.Y) / NumSlots;

	// LocalPosition.Y를 이용해 슬롯 인덱스를 계산합니다.
	int32 SlotIndex = FMath::Clamp(static_cast<int32>((LocalPosition.Y - WeaponAreaMin.Y) / SlotHeight), 0, NumSlots - 1);

	return SlotIndex;
}

