// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlidingDoor::ASlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(Root);
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Root);
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoor::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &ASlidingDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
}

// Called every frame
void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlidingDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EnableInput(PlayerController);
	GetAround = true;
}

void ASlidingDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DisableInput(PlayerController);
	GetAround = false;
	OutSideEvent();
}

void ASlidingDoor::MoveDoor(FVector Location)
{
	FVector CurrentLocation = GetActorLocation();
	if(GetAround)
	{
		SetActorRelativeLocation(CurrentLocation+Location);
	}
}

void ASlidingDoor::AddWidget()
{
	Widget = CreateWidget(GetWorld(), PasswordWidget);
	if (Widget)
	{
		Widget->AddToViewport();
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}
}

void ASlidingDoor::RemoveWidget()
{
	if(Widget)
	{
		Widget->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}
