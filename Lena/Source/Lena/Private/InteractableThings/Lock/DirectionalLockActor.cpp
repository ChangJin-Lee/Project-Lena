// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/Lock/DirectionalLockActor.h"
#include "InteractableThings/Door/SlidingDoorActor.h"
#include "Components/BoxComponent.h"
#include "UI/InteractWidget.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

class UInteractWidget;
// Sets default values
ADirectionalLockActor::ADirectionalLockActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DirectionalLockSceneComponent  = CreateDefaultSubobject<USceneComponent>(TEXT("DirectionalLockSceneComponent"));
	DirectionalLockSceneComponent->SetupAttachment(RootComponent);

	DirectionalLockshackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockshackleMeshComponent"));
	DirectionalLockshackleMeshComponent->SetupAttachment(DirectionalLockSceneComponent);

	DirectionalLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockBodyMeshComponent"));
	DirectionalLockBodyMeshComponent->SetupAttachment(DirectionalLockSceneComponent);

	DirectionalLockBallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionalLockBallMeshComponent"));
	DirectionalLockBallMeshComponent->SetupAttachment(DirectionalLockBodyMeshComponent);
	DirectionalLockBallMeshComponent->SetRelativeLocation(FVector(0,10.0f,0));

	DirectionalLockTimeLine = CreateDefaultSubobject<UTimelineComponent>("DirectionalLockTimeLine");
	bIsTimeLinePlaying = false;
	WidgetComponent->SetRelativeLocation(FVector(0,0,105.0f));
	HitBox->SetRelativeScale3D(FVector::One()*3.5);
}

// Called when the game starts or when spawned
void ADirectionalLockActor::BeginPlay()
{
	Super::BeginPlay();
	
	if(WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		InteractWidget->SetInstructionAtBeginPlay(FText::FromString("Press Arrow Keys"));
	}

	if(DirectionalLockCurve)
	{
		DirectionalLockTimeLineCallback.BindUFunction(this, FName("HandleDirectionalLockProgress"));
		DirectionalLockTimeLine->AddInterpFloat(DirectionalLockCurve, DirectionalLockTimeLineCallback);
		DirectionalLockTimeLineFinishedCallback.BindUFunction(this,FName("HandleDirectionalLockFinished"));
		DirectionalLockTimeLine->SetTimelineFinishedFunc(DirectionalLockTimeLineFinishedCallback);
	}
	
}

// Called every frame
void ADirectionalLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADirectionalLockActor::Unlock(AActor* ActorToUnlock)
{
	Super::Unlock(ActorToUnlock);
	ADoorActor* Door = Cast<ADoorActor>(ActorToUnlock);
	if(Door)
	{
		Door->Open();
	}
}

void ADirectionalLockActor::HandleDirectionalLockProgress(float value)
{
	if(DirectionalLockBallMeshComponent)
	{
		FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, value);
		DirectionalLockBallMeshComponent->SetRelativeLocation(NewLocation);
	}
}

void ADirectionalLockActor::HandleDirectionalLockFinished()
{
	bIsTimeLinePlaying = false;
	CheckRightAnswer();
}

void ADirectionalLockActor::MoveFromStart(FVector InputVector, FString direction)
{
	if(bIsTimeLinePlaying) return;
	
	InitialLocation = DirectionalLockBallMeshComponent->GetRelativeLocation();
	TargetLocation = InitialLocation + InputVector;
	
	if(DirectionalLockTimeLine)
	{
		bIsTimeLinePlaying = true;
		DirectionalLockTimeLine->PlayFromStart();
	}

	InputPassWord += direction;

	// WidgetDisplayPassword += FindObject<UEnum>(ANY_PACKAGE, TEXT("DirectionEnum"), true)->GetDisplayNameTextByValue(FCString::Atoi(*direction)).ToString();
	
	DirectionEnum DirectionEnumClass = static_cast<DirectionEnum>(FCString::Atoi(*direction));
	WidgetDisplayPassword += EnumToString(DirectionEnumClass);
	
	if(WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		InteractWidget->SetInstructionColor(FColor::Yellow);
		InteractWidget->SetInstruction(FText::FromString(WidgetDisplayPassword));
		InteractWidget->InstructionText = FText::FromString(WidgetDisplayPassword);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation(),1.0f, 2.0f);
}

bool ADirectionalLockActor::IsMovementProgressing()
{
	if(bIsTimeLinePlaying)
	{
		return true;
	}
	
	return false;
}

void ADirectionalLockActor::CheckRightAnswer()
{
	if(GetNum(InputPassWord) < 5) return;

	if(CheckPassword(InputPassWord))
	{
		Unlock(TargetDoor);
		DirectionalLockBodyMeshComponent->SetSimulatePhysics(true);
		DirectionalLockshackleMeshComponent->SetSimulatePhysics(true);
		DirectionalLockBallMeshComponent->SetSimulatePhysics(true);
		
		if(WidgetComponent)
		{
			UUserWidget* Widget = WidgetComponent->GetWidget();
			UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
			InteractWidget->SetInstructionColor(FColor::Blue);
			InteractWidget->SetInstruction(FText::FromString(WidgetDisplayPassword));
			InteractWidget->InstructionText = FText::FromString("Congratulations! You solved the puzzle!");
			InteractWidget->SetInstructionColor(FColor::Green);
		}
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), WrongAnswerSound, GetActorLocation());
		if(WidgetComponent)
		{
			UUserWidget* Widget = WidgetComponent->GetWidget();
			UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
			InteractWidget->SetInstructionColor(FColor::Red);
			InteractWidget->SetInstruction(FText::FromString(WidgetDisplayPassword));
			InteractWidget->SetInstruction(FText::FromString("Try Again!"));
			InteractWidget->InstructionText = FText::FromString("Try Again!");
		}

		if(WrongAnswerCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
		}
		InputPassWord = "";
		WidgetDisplayPassword = "";
	}
}

FString ADirectionalLockActor::EnumToString(DirectionEnum EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("DirectionEnum"), true);
	if (!EnumPtr)
	{
		return FString("Invalid");
	}
    
	return EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(EnumValue)).ToString();
}