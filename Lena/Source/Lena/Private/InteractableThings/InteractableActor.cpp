// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/InteractableThings/InteractableActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Text.h"
#include "UI/InteractWidget.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Root);
	
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	HideWidgetComponent();
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
		ShowWidgetComponent();
	}
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DisableInput(PlayerController);
	HideWidgetComponent();
	OutSideEvent();
}

void AInteractableActor::AddWidget(UUserWidget* Widget_)
{
	if (Widget_)
	{
		Widget_->AddToViewport();
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}
}

void AInteractableActor::RemoveWidget(UUserWidget* Widget_)
{
	if(Widget_ && Widget_->IsInViewport())
	{
		Widget_->RemoveFromParent();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

void AInteractableActor::ShowWidgetComponent()
{
	if(WidgetComponent)
	{
		WidgetComponent->SetVisibility(true);
	}
}

void AInteractableActor::HideWidgetComponent()
{
	if(WidgetComponent)
	{
		WidgetComponent->SetVisibility(false);
	}
}

void AInteractableActor::ClearInstructionWidgetTextDelay(float DelayTime)
{
	FTimerDelegate  Timer;
	Timer.BindUFunction(this, FName("WrongAnswerDelayFunction"));

	GetWorld()->GetTimerManager().SetTimer(WrongAnswerDelayHandle, Timer, DelayTime, false);
}

void AInteractableActor::WrongAnswerDelayFunction()
{
	if(WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		InteractWidget->SetInstruction(FText::FromString("Press E"));
		InteractWidget->SetColorAndOpacity(FLinearColor::White);
	}
}

void AInteractableActor::SetInstructionWidgetText(FText Text, FLinearColor Color)
{
	if(WidgetComponent)
	{
		UUserWidget* Widget = WidgetComponent->GetWidget();
		UInteractWidget* InteractWidget = Cast<UInteractWidget>(Widget);
		InteractWidget->SetInstruction(Text);
		InteractWidget->SetColorAndOpacity(Color);
	}
}

