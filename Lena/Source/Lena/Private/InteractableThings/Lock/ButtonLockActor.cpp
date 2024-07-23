// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/Lock/ButtonLockActor.h"

#include "Characters/Base_Character.h"
#include "Components/BoxComponent.h"
#include "Game/Controller/ShooterPlayerController.h"
#include "InteractableThings/Door/DoorActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AButtonLockActor::AButtonLockActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonDataArray.SetNum(ButtonNums);
	ButtonTimelines.SetNum(ButtonNums);
	ButtonMeshComponents.SetNum(ButtonNums);
	
	ButtonLockSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ButtonLockSceneComponent"));
	ButtonLockSceneComponent->SetupAttachment(RootComponent);

	ButtonLockShackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockShackleMeshComponent"));
	ButtonLockShackleMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockBodyMeshComponent"));
	ButtonLockBodyMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockKeypadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockKeypadMeshComponent"));
	ButtonLockKeypadMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	for(int8 i = 0; i < ButtonNums; ++i)
	{
		// ButtonData = NewObject<UButtonLockButtonData>(this);
		ButtonMeshComponents[i] = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("ButtonLockButtonComponent_%d"), i));
		ButtonMeshComponents[i]->SetupAttachment(ButtonLockBodyMeshComponent);
		ButtonMeshComponents[i]->SetRelativeLocation(FVector(0,4,0));
		ButtonTimelines[i] = CreateDefaultSubobject<UTimelineComponent>(*FString::Printf(TEXT("ButtonLockButtonTimeline_%d"), i));
	}

	HitBox->SetRelativeLocation(FVector(0,0,20));
	HitBox->SetRelativeScale3D(FVector::One() * 2.0f);
	CameraMoveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraMoveTimelineComponent"));
}

void AButtonLockActor::Unlock(AActor* ActorToUnlock)
{
	Super::Unlock(ActorToUnlock);
	ADoorActor* Door = Cast<ADoorActor>(ActorToUnlock);
	if(Door)
	{
		Door->RequiredCondition = "Default";
		Door->Open();
	}
}

// Called when the game starts or when spawned
void AButtonLockActor::BeginPlay()
{
	Super::BeginPlay();

	for(int8 i = 0; i < ButtonNums; ++i)
	{
		if(ButtonTimelines[i])
		{
			FOnTimelineFloatStatic ButtonLockButtonMoveStartCallback;
			ButtonLockButtonMoveStartCallback.BindUFunction(this, FName("HandleButtonLockProgress"), i);
			ButtonTimelines[i]->AddInterpFloat(ButtonLockCurve, ButtonLockButtonMoveStartCallback);
			
			FOnTimelineEvent ButtonLockButtonMoveFinishedCallback;
			// ButtonLockButtonMoveFinishedCallback.CreateUFunction(this, FName("HandleButtonLockFinished"), i);
			ButtonLockButtonMoveFinishedCallback.BindUFunction(this, FName("HandleButtonLockFinished"));
			ButtonTimelines[i]->SetTimelineFinishedFunc(ButtonLockButtonMoveFinishedCallback);
		}
	}
 
	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	
	if(PlayerController)
	{
		PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
		PlayerCameraManager = PlayerController->PlayerCameraManager;
		if(PlayerCameraManager)
		{
			ZoomedCameraLocation = GetActorLocation() + GetActorRightVector() * 100.0f * GetActorScale() + FVector(0, 0.f, 10.0f) *  GetActorScale();
			ZoomedCameraRotation = GetActorRotation();
			ZoomedCameraRotation.Yaw += -90.0f;
			ZoomedCameraRotation.Pitch += -10.0f;

			CameraTargetLocationActor = CreateCameraMoveTargetLocationActor(ZoomedCameraLocation, ZoomedCameraRotation);
		}
	}
}

// Called every frame
void AButtonLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AButtonLockActor::HandleButtonLockProgress(float value, int32 ButtonIndex)
{
	if (ButtonIndex == INDEX_NONE || ButtonIndex >= ButtonDataArray.Num())
	{
		return;
	}
	
	FVector NewLocation = FMath::Lerp(ButtonDataArray[ButtonIndex].InitialLocation, ButtonDataArray[ButtonIndex].TargetLocation, value);
	ButtonMeshComponents[ButtonIndex]->SetRelativeLocation(NewLocation);
}

void AButtonLockActor::MoveButton(UStaticMeshComponent* TargetMeshComponent)
{
	int32 FindIndex = ButtonMeshComponents.IndexOfByKey(TargetMeshComponent);
	
	if (FindIndex == INDEX_NONE)
	{
		return;
	}

	for(FButtonLockButtonData otherButtonMove : ButtonDataArray)
	{
		if(otherButtonMove.bIsButtonMove == true)
			return;
	}

	SelectedButtonIndex = FindIndex;
	ButtonDataArray[FindIndex].InitialLocation = TargetMeshComponent->GetRelativeLocation();
	ButtonDataArray[FindIndex].bIsButtonMove = true;
	
	ButtonDataArray[FindIndex].IsCliked == true ?  ButtonMovePlayFromStart(FindIndex, 4) : ButtonMovePlayFromStart(FindIndex, -4);
	ButtonDataArray[FindIndex].IsCliked = !ButtonDataArray[FindIndex].IsCliked;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation());

	if(CheckPassword(GetPressedPassword()))
	{
		OpenLock();
	}
}

void AButtonLockActor::ButtonMovePlayFromStart(int32 index, float value)
{
	ButtonDataArray[index].TargetLocation = ButtonDataArray[index].InitialLocation + FVector(0,value,0);
	ButtonTimelines[index]->PlayFromStart();
}

void AButtonLockActor::HandleButtonLockFinished()
{
	if (SelectedButtonIndex == INDEX_NONE || SelectedButtonIndex >= ButtonDataArray.Num())
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("HandleButtonLockFinished :  %d"), SelectedButtonIndex);
	ButtonDataArray[SelectedButtonIndex].bIsButtonMove = false;
}

void AButtonLockActor::ZoomInCamera()
{
	if(bIsZoomedIn) return;
	
	LockCamera();
	bIsZoomedIn = true;

	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorHiddenInGame(true);
	}
	
	if (CameraTargetLocationActor)
	{
		MoveCamera(CameraTargetLocationActor);
	}
}

void AButtonLockActor::ZoomOutCamera()
{
	if(bIsZoomedIn) return;
	bIsZoomedIn = true;
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorHiddenInGame(false);
	}
	MoveCamera(PlayerController->GetPawn());
	UnLockCameara();
}

void AButtonLockActor::LockCamera()
{
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetInputMode(FInputModeGameAndUI());

		AShooterPlayerController* shooter_player_controller = Cast<AShooterPlayerController>(PlayerController);
		if(shooter_player_controller)
		{
			shooter_player_controller->EnableMouseClick();
		}
	}
}

void AButtonLockActor::UnLockCameara()
{
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		
		AShooterPlayerController* shooter_player_controller = Cast<AShooterPlayerController>(PlayerController);
		if(shooter_player_controller)
		{
			shooter_player_controller->DisableMouseClick();
		}
	}
}

// Create Target Actor for SetViewTargetWithBlend in Camera Zoomin
AActor* AButtonLockActor::CreateCameraMoveTargetLocationActor(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* CameraTargetActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), SpawnParams);

	if (CameraTargetActor)
	{
		USceneComponent* SceneComponent = NewObject<USceneComponent>(CameraTargetActor);
		if (SceneComponent)
		{
			CameraTargetActor->SetRootComponent(SceneComponent);
			SceneComponent->RegisterComponent();
			SceneComponent->SetWorldLocation(Location);
			SceneComponent->SetWorldRotation(Rotation);
		}
	}
	return CameraTargetActor;
}

void AButtonLockActor::MoveCamera(AActor* TargetActor)
{
	if (PlayerController)
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 1.0f; // Adjust transition time as needed
		PlayerController->SetViewTargetWithBlend(TargetActor, TransitionParams.BlendTime, VTBlend_Cubic);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&AButtonLockActor::CameraMoveFinished,
			TransitionParams.BlendTime,
			false
			);
	}
}


void AButtonLockActor::CameraMoveFinished()
{
	bIsZoomedIn = false;
}

FString AButtonLockActor::GetPressedPassword()
{
	FString string = "";
	for(int32 i = 0; i < ButtonNums; ++i)
	{
		if(ButtonDataArray[i].IsCliked)
		{
			string+=FString::FromInt(i+1);
		}
	}

	return string;
}


void AButtonLockActor::OpenLock()
{
	Unlock(TargetDoor);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AButtonLockActor::DestroyButtonLock,
		1.0f,
		false
		);
	
	ButtonLockShackleMeshComponent->SetSimulatePhysics(true);
	ButtonLockBodyMeshComponent->SetSimulatePhysics(true);
	ButtonLockKeypadMeshComponent->SetSimulatePhysics(true);
	for(int32 i = 0; i < ButtonNums; ++i)
	{
		ButtonMeshComponents[i]->SetSimulatePhysics(true);
	}
	WidgetComponent->SetVisibility(false);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RightAnswerSound, GetActorLocation());
}

void AButtonLockActor::DestroyButtonLock()
{
	ZoomOutCamera();
	HitBox->DestroyComponent();
}
