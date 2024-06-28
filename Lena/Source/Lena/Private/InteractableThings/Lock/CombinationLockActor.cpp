// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/Lock/CombinationLockActor.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractWidget.h"

// Sets default values
ACombinationLockActor::ACombinationLockActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WheelSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WheelSceneComponent"));
	WheelSceneComponent->SetupAttachment(RootComponent);

	WheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh"));
	WheelMesh->SetupAttachment(WheelSceneComponent);
	WheelMesh->SetRelativeLocation(FVector(-45,0,0));
	WheelMesh->SetRelativeScale3D(FVector(5.0f,5.0f,5.0f));
	
	WheelMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh1"));
	WheelMesh1->SetupAttachment(WheelSceneComponent);
	WheelMesh1->SetRelativeLocation(FVector(-15,0,0));
	WheelMesh1->SetRelativeScale3D(FVector(5.0f,5.0f,5.0f));

	WheelMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh2"));
	WheelMesh2->SetupAttachment(WheelSceneComponent);
	WheelMesh2->SetRelativeLocation(FVector(15,0,0));
	WheelMesh2->SetRelativeScale3D(FVector(5.0f,5.0f,5.0f));

	WheelMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelMesh3"));
	WheelMesh3->SetupAttachment(WheelSceneComponent);
	WheelMesh3->SetRelativeLocation(FVector(45,0,0));
	WheelMesh3->SetRelativeScale3D(FVector(5.0f,5.0f,5.0f));

	CombinationLockTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CombinationLockTimeline"));
}

// Called when the game starts or when spawned
void ACombinationLockActor::BeginPlay()
{
	Super::BeginPlay();

	if(WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		InteractWidget->SetInstructionAtBeginPlay(FText::FromString("Drag Mouse Wheel"));
	}

	if (CombinationLockCurve)
	{
		CombinationLockTimelineCallback.BindUFunction(this, FName("HandleCombinationLockProgress"));
		CombinationLockTimeline->AddInterpFloat(CombinationLockCurve, CombinationLockTimelineCallback);
	}
}

// Called every frame
void ACombinationLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombinationLockActor::ScrollCombinationLock(FRotator InTargetRotation)
{
	InitialRotation = WheelMesh->GetRelativeRotation();
	TargetRotation = InitialRotation + InTargetRotation;
	if (CombinationLockTimeline)
	{
		CombinationLockTimeline->PlayFromStart();
	}
}

void ACombinationLockActor::HandleCombinationLockProgress(float Value)
{
	FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, Value);
	WheelMesh->SetRelativeRotation(NewRotation);
}