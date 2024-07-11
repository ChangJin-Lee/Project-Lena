// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "InteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class LENA_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInteractWidget(const FObjectInitializer& ObjectInitializer);

	FText InstructionText = FText::FromString("Press E");
	UFUNCTION(BlueprintCallable)
	void SetInstruction(FText Input);
	UFUNCTION(BlueprintCallable)
	void SetInstructionAtBeginPlay(FText Input);
	UFUNCTION(BlueprintCallable)
	void SetInstructionColor(FLinearColor Input);
	FText GetInstruction();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Instructions0;
};
