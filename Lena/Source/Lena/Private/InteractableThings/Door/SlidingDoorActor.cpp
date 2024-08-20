// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/SlidingDoorActor.h"

#include "CanvasTypes.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
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
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorOpenSound, GetActorLocation());
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
    SetInstructionWidgetText(FText::FromString("Open!"),FLinearColor::Green);
    
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation(),1.0f, 2.0f);
}


void ASlidingDoorActor::WrongAnswer()
{
    SetInstructionWidgetText(FText::FromString("Locked!"),FLinearColor::Red);
    ClearInstructionWidgetTextDelay(0.5);
    
    if(WrongAnswerCameraShakeClass)
    {
        GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
    }

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), WrongAnswerSound, GetActorLocation(),1.0f, 2.0f);
}