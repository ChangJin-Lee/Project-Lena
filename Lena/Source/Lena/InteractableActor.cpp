// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Text.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Root);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("AlertText"));
	TextRenderComponent->SetupAttachment(Root);
	TextRenderComponent->SetText(FText::FromString("Press E"));
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	HideTextRenderComponent();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EnableInput(PlayerController);
	if(!IsDone)
	{
		ShowTextRenderComponent();
	}
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DisableInput(PlayerController);
	HideTextRenderComponent();
	OutSideEvent();
}

void AInteractableActor::AddWidget()
{
	Widget = CreateWidget(GetWorld(), PasswordWidget);
	if (Widget)
	{
		Widget->AddToViewport();
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}
}

void AInteractableActor::RemoveWidget()
{
	if(Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}


void AInteractableActor::ShowTextRenderComponent()
{
	TextRenderComponent->SetVisibility(true);
}

void AInteractableActor::HideTextRenderComponent()
{
	TextRenderComponent->SetVisibility(false);
}

