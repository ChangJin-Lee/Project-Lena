// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

#include "Base_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


ADoorActor::ADoorActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorActor::OpenSlidingDoor(FVector Location)
{
	MeshComponent->SetRelativeLocation(Location);
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