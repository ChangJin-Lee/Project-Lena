// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/DoorActor.h"

#include "Lena/Public/Characters/Base_Character.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"



ADoorActor::ADoorActor()
{
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

void ADoorActor::OpenSlidingDoor(FVector Location)
{
	if(MeshComponent)
	{
		MeshComponent->SetRelativeLocation(Location);
	}
}

bool ADoorActor::CheckRequiredItem()
{
	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character && Character->HasItemInInventory(RequiredItem))
	{
		return true;
	}
	
	if(WrongAnswerCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
	}
	
	return false;
}

void ADoorActor::OpenHingedDoor(FRotator Rotation)
{
	if(MeshComponent)
	{
		MeshComponent->SetRelativeRotation(Rotation);
	}
}