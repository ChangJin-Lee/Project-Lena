// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableThings/Lock/ButtonLockActor.h"

#include "Camera/CameraComponent.h"
#include "Characters/Base_Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AButtonLockActor::AButtonLockActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ButtonLockButtonMeshComponents.SetNum(8);
	ClikedButtons.SetNum(8);
	
	ButtonLockSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ButtonLockSceneComponent"));
	ButtonLockSceneComponent->SetupAttachment(RootComponent);

	ButtonLockShackleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockShackleMeshComponent"));
	ButtonLockShackleMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockBodyMeshComponent"));
	ButtonLockBodyMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	ButtonLockKeypadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonLockKeypadMeshComponent"));
	ButtonLockKeypadMeshComponent->SetupAttachment(ButtonLockSceneComponent);

	uint8 Counter = 0;
	for(UStaticMeshComponent* &ButtonLockButtonComponent : ButtonLockButtonMeshComponents)
	{
		ClikedButtons[Counter] = false;
		FString ComponentName = FString::Printf(TEXT("ButtonLockButtonComponent_%d"), Counter++);
		ButtonLockButtonComponent = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
		ButtonLockButtonComponent->SetupAttachment(ButtonLockBodyMeshComponent);
		ButtonLockButtonComponent->SetRelativeLocation(FVector(0,4,0));
	}

	ButtonLockTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ButtonLockTimelineComponent"));
	HitBox->SetRelativeLocation(FVector(0,0,20));
	HitBox->SetRelativeScale3D(FVector::One() * 2.0f);

	CameraMoveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraMoveTimelineComponent"));
}

// Called when the game starts or when spawned
void AButtonLockActor::BeginPlay()
{
	Super::BeginPlay();

	if(ButtonLockTimelineComponent)
	{
		ButtonLockButtonCallback.BindUFunction(this, FName("HandleButtonLockProgress"));
		ButtonLockTimelineComponent->AddInterpFloat(ButtonLockCurve, ButtonLockButtonCallback);
	}
	
	if(CameraMoveTimelineComponent)
	{
		CameraMoveCallback.BindUFunction(this, FName("HandleCameraMoveProgress"));
		CameraMoveFinishedCallback.BindUFunction(this, FName("HandleCameraMoveFinished"));
		CameraMoveTimelineComponent->AddInterpFloat(CameraMoveCurve, CameraMoveCallback);
		CameraMoveTimelineComponent->SetTimelineFinishedFunc(CameraMoveFinishedCallback);
	}

	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	
	PlayerCameraManager = PlayerController->PlayerCameraManager;
	if(PlayerController)
	{
		if(PlayerCameraManager)
		{
			InitialCameraLocation = PlayerCameraManager->GetCameraLocation();
			InitialCameraRotation = PlayerCameraManager->GetCameraRotation();

			ZoomedCameraLocation = GetActorLocation() + GetActorRightVector() * 150.0f + FVector(50.f, 0.f, 0);
			ZoomedCameraRotation = GetActorRotation();
			ZoomedCameraRotation.Yaw += -90.0f;

			CameraInitialLocationActor = CreateTargetActor(InitialCameraLocation, InitialCameraRotation);
			CameraTargetLocationActor = CreateTargetActor(ZoomedCameraLocation, ZoomedCameraRotation);
			
			UE_LOG(LogTemp, Warning, TEXT("GetActorLocation() : %s "), *GetActorLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("CameraTargetLocationActor : %s "), *CameraTargetLocationActor->GetActorLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("CameraInitialLocationActor : %s "), *CameraInitialLocationActor->GetActorLocation().ToString());
		}
	}
}

// Called every frame
void AButtonLockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AButtonLockActor::HandleButtonLockProgress(float value)
{
	FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, value);
	ButtonLockButtonMeshComponents[SelectedButtonIndex]->SetRelativeLocation(NewLocation);
}

void AButtonLockActor::MoveButton(UStaticMeshComponent* TargetMeshComponent)
{
	int32 FindIndex = ButtonLockButtonMeshComponents.Find(TargetMeshComponent);

	if(FindIndex != INDEX_NONE)
	{
		InitialLocation = TargetMeshComponent->GetRelativeLocation();
		SelectedButtonIndex = FindIndex;
		ButtonLockTimelineComponent->PlayFromStart();
	}
}

void AButtonLockActor::HandleCameraMoveProgress(float value)
{
	if(!PlayerCameraManager) return;

	FVector NewLocation = FMath::Lerp(InitialCameraLocation, ZoomedCameraLocation, value);
	FRotator NewRotation = FMath::Lerp(InitialCameraRotation, ZoomedCameraRotation, value);

	PlayerCameraManager->SetActorLocation(NewLocation);
	PlayerCameraManager->SetActorRotation(NewRotation);
}

void AButtonLockActor::HandleCameraMoveFinished()
{
	if (!bIsZoomedIn)
	{
		UnLockCameara();
	}
}

void AButtonLockActor::ZoomInCamera()
{
	// if(bIsZoomedIn || !PlayerCameraManager) return;

	UE_LOG(LogTemp, Warning, TEXT("ZoomInCamera() "));
	
	// bIsZoomedIn = true;
	LockCamera();
	// CameraMoveTimelineComponent->PlayFromStart();

	CameraInitialLocationActor->SetActorLocation(PlayerCameraManager->GetCameraLocation());
	CameraInitialLocationActor->SetActorRotation(PlayerCameraManager->GetCameraRotation());
	
	if (PlayerController && CameraTargetLocationActor)
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 1.0f; // Adjust transition time as needed
		PlayerController->SetViewTargetWithBlend(CameraTargetLocationActor, TransitionParams.BlendTime, VTBlend_Cubic);
	}
}


void AButtonLockActor::ZoomOutCamera()
{
	if(bIsZoomedIn || !PlayerCameraManager) return;

	bIsZoomedIn = true;

	if (PlayerController && CameraInitialLocationActor)
	{
		FViewTargetTransitionParams TransitionParams;
		TransitionParams.BlendTime = 1.0f; // Adjust transition time as needed
		PlayerController->SetViewTargetWithBlend(CameraInitialLocationActor, TransitionParams.BlendTime, VTBlend_Cubic);
	}
	UnLockCameara();
}

void AButtonLockActor::LockCamera()
{
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
	}
}

void AButtonLockActor::UnLockCameara()
{
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
	}
}



AActor* AButtonLockActor::CreateTargetActor(FVector Location, FRotator Rotation)
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
