// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Items/Base_Item.h"

#include "ImageUtils.h"
#include "TextureResource.h"
#include "Characters/Base_Character.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"

// Sets default values
ABase_Item::ABase_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(MeshComponent);

	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->SetupAttachment(RootComponent);
	SceneCaptureComponent2D->bCaptureEveryFrame = false;
	SceneCaptureComponent2D->bCaptureOnMovement = false;
}

// Called when the game starts or when spawned
void ABase_Item::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABase_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Item::PickUp()
{
	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		Character->PickupItem(this, EInventorySlotType::ISt_Ground);
		// UE_LOG(LogTemp, Warning, TEXT("Pick up item %s"), *ItemDetails->GetName());
		
		// 아이템을 비활성화하거나 제거
		// SetActorHiddenInGame(true);
		// SetActorEnableCollision(false);
		// SetActorTickEnabled(false);
		
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
		}
	}
}