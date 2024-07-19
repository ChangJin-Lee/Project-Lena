// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/SlidingDoorActor.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ArchiveReplaceObjectRef.h"
#include "UI/InteractWidget.h"

ASlidingDoorActor::ASlidingDoorActor()
{
    SlidingDoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SlidingDoorTimeline"));
}

void ASlidingDoorActor::BeginPlay()
{
    Super::BeginPlay();

    if (SlidingDoorCurve)
    {
        SlidingDoorTimelineCallback.BindUFunction(this, FName("HandleSlidingDoorProgress"));
        SlidingDoorTimeline->AddInterpFloat(SlidingDoorCurve, SlidingDoorTimelineCallback);
    }
}

void ASlidingDoorActor::Open()
{
    InitialLocation = MeshComponent->GetRelativeLocation();
    TargetLocation = FVector(0,130,0);
    if (SlidingDoorTimeline)
    {
        SlidingDoorTimeline->PlayFromStart();
    }
}

void ASlidingDoorActor::OpenSlidingDoor(FVector InTargetLocation)
{
    InitialLocation = MeshComponent->GetRelativeLocation();
    TargetLocation = InTargetLocation;
    if (SlidingDoorTimeline)
    {
        SlidingDoorTimeline->PlayFromStart();
    }
}

void ASlidingDoorActor::HandleSlidingDoorProgress(float Value)
{
    FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Value);
    MeshComponent->SetRelativeLocation(NewLocation);
}

void ASlidingDoorActor::RightAnswer(FVector InTargetLocation)
{
    OpenSlidingDoor(InTargetLocation);
    IsDone = true;
    RemoveWidget(WidgetComponent->GetWidget());

    if(WidgetComponent)
    {
        UUserWidget* Widget = WidgetComponent->GetWidget();
        UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
        InteractWidget->SetInstruction(FText::FromString("Open!"));
    }
}


void ASlidingDoorActor::WrongAnswer()
{
    if(WidgetComponent)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation());
        UUserWidget* Widget = WidgetComponent->GetWidget();
        UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
        InteractWidget->SetInstruction(FText::FromString("Need Key"));
        InteractWidget->SetColorAndOpacity(FLinearColor::Red);
        StartWrongAnswerDelay(0.5f);
    }
}

void ASlidingDoorActor::StartWrongAnswerDelay(float DelayTime)
{
    FTimerDelegate  Timer;
    Timer.BindUFunction(this, FName("WrongAnswerDelayFunction"));

    GetWorld()->GetTimerManager().SetTimer(WrongAnswerDelayHandle, Timer, DelayTime, false);
}

void ASlidingDoorActor::WrongAnswerDelayFunction()
{
    if(WidgetComponent)
    {
        UUserWidget* Widget = WidgetComponent->GetWidget();
        UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
        InteractWidget->SetColorAndOpacity(FLinearColor::White);
    }
}
