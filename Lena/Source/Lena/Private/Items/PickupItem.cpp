// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "Lena/Characters/Base_Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(MeshComponent);

	// ItemDetails는 SpawnActor를 통해 생성해야 합니다.
	ItemDetails = nullptr;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
	// BeginPlay에서 ItemDetails를 생성합니다.
	ItemDetails = GetWorld()->SpawnActor<ABase_Item>(ABase_Item::StaticClass());
	if (ItemDetails)
	{
		ItemDetails->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::PickUp()
{
	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		Character->AddItemToInventory(ItemDetails);
		UE_LOG(LogTemp, Warning, TEXT("Pick up item %s"), *ItemDetails->GetName());
		
		// 아이템을 비활성화하거나 제거
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
		}
	}
}

void APickupItem::SetThisItemName(FName NewName)
{
	if(ItemDetails)
	{
		ItemDetails->ItemName = NewName;
	}
}