// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InteractWidget.h"

UInteractWidget::UInteractWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CanvasPanel && Instructions0)
	{
		Instructions0->SetText(InstructionText);
		// 텍스트 블록의 스타일이나 위치 등을 설정할 수 있습니다.
	}
}

void UInteractWidget::SetInstruction(FText Input)
{
	Instructions0->SetText(Input);
}

void UInteractWidget::SetInstructionAtBeginPlay(FText Input)
{
	InstructionText = Input;
}

FText UInteractWidget::GetInstruction()
{
	return Instructions0->GetText();
}

void UInteractWidget::SetInstructionColor(FLinearColor Input)
{
	Instructions0->SetColorAndOpacity(Input);
}
