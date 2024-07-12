// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/Door/DoorActor.h"

#include "Lena/Public/Characters/Base_Character.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
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

bool ADoorActor::CheckRequiredItem()
{
	if(RequiredItemDescription == "Default")
	{
		return true;
	}

	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character && Character->Inventory->FindItemByDescription(RequiredItemDescription) != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("if (Character && Character->Inventory->FindItemByDescription(RequiredItemDescription) != INDEX_NONE)"));
		return true;
	}
	
	if(WrongAnswerCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(WrongAnswerCameraShakeClass);
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