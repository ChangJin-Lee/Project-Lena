// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Game/Controller/ShooterPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Base_Character.h"
#include "InteractableThings/Lock/ButtonLockActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InventoryWidget.h"

AShooterPlayerController::AShooterPlayerController()
{
	//static ConstructorHelpers::FObjectFinder<UInventoryWidget> InventoryWidgetFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/HUD/WBP_InventoryMain.WBP_InventoryMain'"));
	//if(InventoryWidgetFinder.Succeeded())
	//{
	//	InventoryWidget = InventoryWidgetFinder.Object;
	//}
}


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}

	// 향상된 입력을 받는다. 이게 우리가 평소에 하던 입력에 들어가서 + 버튼을 누르고 액션을 추가해주던 것임
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(IMC_Interaction, 0);
	}

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MouseClick, ETriggerEvent::Triggered, this, &AShooterPlayerController::HandleMouseClick);
		EnhancedInputComponent->BindAction(IA_PickUpItem, ETriggerEvent::Triggered, this, &AShooterPlayerController::HandlePickUpItem);
	}

	// Character Reference caching
	Base_Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
	// Item Info Widget AddViewport
	ItemInfoWidget->AddToViewport();
	ItemInfoWidget->SetInstructionAtBeginPlay(FText::FromString(""), FLinearColor::White);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckPickUpItem, this, &AShooterPlayerController::CheckPickUpItemLine, 0.1f, true);
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromParent();
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::EnableMouseClick()
{
	bIsClickEnabled = true;
}

void AShooterPlayerController::DisableMouseClick()
{
	bIsClickEnabled = false;
}

void AShooterPlayerController::HandleMouseClick()
{
	if(!bIsClickEnabled) return;
	
	float MouseX, MouseY;
	this->GetMousePosition(MouseX, MouseY);

	FVector2D ScreenPosition(MouseX, MouseY);
	FVector WorldLocation, WorldDirection;
	if (this->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = WorldLocation + (WorldDirection * 1000.0f); // Adjust the length of the ray as needed

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			AActor* HitActor = HitResult.GetActor();
			if(HitActor)
			{

				ABase_Character* HitCharacter = Cast<ABase_Character>(HitActor);
				if (HitCharacter)
				{
					// Ignore the character and perform another line trace
					Params.AddIgnoredActor(HitActor);
					if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
					{
						HitActor = HitResult.GetActor();
					}
					else
					{
						HitActor = nullptr; // No valid actor hit
					}
				}
				
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
				AButtonLockActor* ButtonLockActor = Cast<AButtonLockActor>(HitActor);
				if(ButtonLockActor)
				{
					UE_LOG(LogTemp, Warning, TEXT("In Actor: %s"), *HitResult.GetComponent()->GetName());
					UStaticMeshComponent* ButtonMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
					ButtonLockActor->MoveButton(ButtonMeshComponent);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("no Actor: %s"), *HitResult.GetComponent()->GetName());
				}
			}
		}
		// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
	}
}


///// Player Widget
void AShooterPlayerController::InputModeUI()
{
	bShowMouseCursor = true;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	SetInputMode(FInputModeGameAndUI());
	IsInWidget = true;
}


void AShooterPlayerController::InputModeGame()
{
	bShowMouseCursor = false;
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
	SetInputMode(FInputModeGameOnly());
	IsInWidget = false;
}

void AShooterPlayerController::HandlePickUpItem()
{
	UE_LOG(LogTemp, Warning, TEXT("HandlePickUpItem"));

	if(LineTraceItem && Base_Character)
	{
		Base_Character->PickupItem(LineTraceItem);
	}
}


void AShooterPlayerController::CheckPickUpItemLine()
{
	FVector Start = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetActorForwardVector() * 30.0f);;
	FVector End = PlayerCameraManager->GetCameraLocation() + (PlayerCameraManager->GetActorForwardVector() * 400.0f); // Adjust the length of the ray as needed

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	float Radius = 15.0f;

	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility,FCollisionShape::MakeSphere(Radius), Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if(HitActor)
		{
			ABase_Item* Item = Cast<ABase_Item>(HitActor);
			if(Item)
			{
				ItemInfoWidget->SetInstruction(FText::FromString(Item->ItemName));
				LineTraceItem = Item;
				return;
			}
		}
	}
	
	ItemInfoWidget->SetInstruction(FText::FromString(""));
	LineTraceItem = nullptr;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
	// DrawDebugSphere(GetWorld(), End, Radius, 3, FColor::Red, false, 1.0f, 0, 1.0f);
}

void AShooterPlayerController::CheckPickUpItemSweep()
{
	ACharacter* Character_ =  UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	FVector Start = Character_->GetActorLocation();
	FVector End = Character_->GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	// Params.AddIgnoredActor(Character_);
	float Radius = 130.0f;

	if(GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params))
	{
		Base_Character->GroundItemsComponent->Items.Empty();
		
		for(FHitResult HitResult : HitResults )
		{
			AActor* HitActor = HitResult.GetActor();
			if(HitActor)
			{
				ABase_Item* Item = Cast<ABase_Item>(HitActor);
				if(Item)
				{
					if(Base_Character)
					{
						Base_Character->CheckGroundItem(Item);
					}
				}
			}
		}
	}
	// DrawDebugSphere(GetWorld(), Start, Radius, 10, FColor::Green, false, 5.0f, 0, 1.0f);
}
