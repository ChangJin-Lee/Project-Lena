// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableThings/Portal/PortalActor.h"
#include "Lena/Public/Game/Controller/ShooterPlayerController.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractWidget.h"


// Sets default values
APortalActor::APortalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	SetInstructionWidgetText(FText::FromString("Press E To Restart!"), FLinearColor::Blue);
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortalActor::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APortalActor::MoveCharacterToLocation()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorLocation(TargetLocation);
	}
}
