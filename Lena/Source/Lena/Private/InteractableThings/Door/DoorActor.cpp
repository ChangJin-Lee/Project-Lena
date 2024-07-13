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
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassFinder(TEXT("Blueprint'/Game/BluePrints/Camera/CameraShake/BP_WrongAnswerCameraShake.BP_WrongAnswerCameraShake_C'"));
	if(CameraShakeClassFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find WrongAnswerCameraShakeClass : %s"), *CameraShakeClassFinder.GetReferencerName());
		WrongAnswerCameraShakeClass = CameraShakeClassFinder.Class;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load WrongAnswerCameraShakeClass"));
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> OpenSoundFinder(TEXT("/Script/Engine.SoundWave'/Game/SFX/Hinged_Door_Open.Hinged_Door_Open'"));
	if(OpenSoundFinder.Succeeded())
	{
		DoorOpenSound = OpenSoundFinder.Object;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load DoorOpenSound"));
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase> CloseSoundFinder(TEXT("/Script/Engine.SoundWave'/Game/SFX/Hinged_Door_Close.Hinged_Door_Close'"));
	if(CloseSoundFinder.Succeeded())
	{
		DoorCloseSound = CloseSoundFinder.Object;
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load DoorCloseSound"));
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
		return true;
	}

	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character && Character->Inventory->FindItemByDescription(RequiredItemDescription) != INDEX_NONE)
	{
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
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorOpenSound, GetActorLocation());
}

void ADoorActor::Close()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorCloseSound, GetActorLocation());
}

void ADoorActor::OpenFail()
{
	if(WrongAnswerCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
	}
	UUserWidget* SearchWidget = WidgetComponent->GetWidget();
	if(SearchWidget)
	{
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(SearchWidget);
		if(InteractWidget)
		{
			InteractWidget->SetInstruction(FText::FromString("Door Locked!"));
			InteractWidget->SetInstructionColor(FLinearColor::Red);
		}
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorCloseSound, GetActorLocation());
}
