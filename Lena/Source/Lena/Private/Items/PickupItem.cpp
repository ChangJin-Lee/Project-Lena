// Fill out your copyright notice in the Description page of Project Settings.


#include "Lena/Public/Items/PickupItem.h"

#include "Lena/Public/Characters/Base_Character.h"
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

	PickupableItemComponent = CreateDefaultSubobject<UPickupableItemComponent>(TEXT("PickupableItemComponent"));
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItem::PickUp(AActor* InteractingActor)
{
	ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Character)
	{
		Character->PickupItem(PickupableItemComponent->ItemID, PickupableItemComponent->Quantity);
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