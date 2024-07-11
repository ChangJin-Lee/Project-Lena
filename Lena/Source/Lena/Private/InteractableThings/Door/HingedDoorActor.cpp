// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/HingedDoorActor.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AHingedDoorActor::AHingedDoorActor()
{
    HingedDoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HingedDoorTimeline"));
}

void AHingedDoorActor::BeginPlay()
{
    Super::BeginPlay();

    if (HingedDoorCurve)
    {
        HingedDoorTimelineCallback.BindUFunction(this, FName("HandleHingedDoorProgress"));
        HingedDoorTimeline->AddInterpFloat(HingedDoorCurve, HingedDoorTimelineCallback);
    }
}

void AHingedDoorActor::OpenHingedDoor(FRotator InTargetRotation)
{
    InitialRotation = MeshComponent->GetRelativeRotation();
    TargetRotation = InTargetRotation;
    if (HingedDoorTimeline)
    {
        HingedDoorTimeline->PlayFromStart();
    }
}

void AHingedDoorActor::CloseHingedDoor(FRotator InTargetRotation)
{
    if (HingedDoorTimeline)
    {
        HingedDoorTimeline->Reverse();
    }
}

void AHingedDoorActor::HandleHingedDoorProgress(float Value)
{
    FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, Value);
    MeshComponent->SetRelativeRotation(NewRotation);
}