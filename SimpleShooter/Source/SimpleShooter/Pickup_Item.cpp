// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Item.h"
#include "Components/BoxComponent.h"

// Sets default values
APickup_Item::APickup_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(MeshComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent -> SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void APickup_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

