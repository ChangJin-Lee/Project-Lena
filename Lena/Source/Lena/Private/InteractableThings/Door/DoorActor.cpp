// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/DoorActor.h"

#include "Lena/Public/Characters/Base_Character.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractWidget.h"


ADoorActor::ADoorActor()
{
	// Class를 찾을때는 오브젝트와 조금 다름
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassFinder(TEXT("/Game/BluePrints/Camera/CameraShake/BP_WrongAnswerCameraShake.BP_WrongAnswerCameraShake_C"));
	if(CameraShakeClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find WrongAnswerCameraShakeClass : %s"), *CameraShakeClassFinder.GetReferencerName());
		WrongAnswerCameraShakeClass = CameraShakeClassFinder.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> OpenSoundFinder(TEXT("/Script/Engine.SoundWave'/Game/SFX/Hinged_Door_Open.Hinged_Door_Open'"));
	if(OpenSoundFinder.Succeeded())
	{
		DoorOpenSound = OpenSoundFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> CloseSoundFinder(TEXT("/Script/Engine.SoundWave'/Game/SFX/Hinged_Door_Close.Hinged_Door_Close'"));
	if(CloseSoundFinder.Succeeded())
	{
		DoorCloseSound = CloseSoundFinder.Object;
	}
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	FrameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMeshComponent"));
	FrameMeshComponent->SetupAttachment(RootComponent);

	NumpadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NumpadWidgetComponent"));
	NumpadWidgetComponent->SetupAttachment(RootComponent);
	NumpadWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	NumpadWidgetComponent->SetVisibility(false);
}

bool ADoorActor::CheckRequiredItem()
{
	// Default door Just Open
	if(RequiredItemDescription == "Default")
	{
		IsDoorOpen = true;
		return true;
	}

	if(IsDoorOpen)
	{
		return true;
	}

	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character && Character->Inventory->FindItemByDescription(RequiredItemDescription) != INDEX_NONE)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		if(Widget)
		{
			UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
			if(InteractWidget)
			{
				IsDoorOpen = true;
				InteractWidget->SetInstructionAtBeginPlay(FText::FromString("Already Opened!"), FLinearColor::Green);
			}
		}
		return true;
	}
	
	return false;
}

FString ADoorActor::GetPassWord()
{
	return PassWord;
}

void ADoorActor::Open()
{
	UInteractWidget* Widget = Cast<UInteractWidget>(WidgetComponent);
	if(Widget)
	{
		Widget->SetInstruction(FText::FromString("Open!"));
		Widget->SetInstructionColor(FLinearColor::Green);
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorOpenSound, GetActorLocation());
}

void ADoorActor::Close()
{
	UInteractWidget* Widget = Cast<UInteractWidget>(WidgetComponent);
	if(Widget)
	{
		Widget->SetInstruction(FText::FromString("Close!"));
		Widget->SetInstructionColor(FLinearColor::Green);
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorCloseSound, GetActorLocation());
}

void ADoorActor::OpenFail()
{
	if(WrongAnswerCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
	}

	UUserWidget* Widget = WidgetComponent->GetWidget();
	if(Widget)
	{
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		if(InteractWidget)
		{
			InteractWidget->DisplayInstructionWithSeconds(FText::FromString("Door Locked!"), FLinearColor::Red, 1.0f);
		}
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorCloseSound, GetActorLocation());
}