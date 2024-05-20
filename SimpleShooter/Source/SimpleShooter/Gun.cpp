// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// SetRootComponent(Root); RootComponent = Root;의 차이에 대해서 잘 알아두자.
	SetRootComponent(Root);
	// RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	FHitResult HitResult;
	FVector ShotDirection;

	bool bSuccess = GunTrace(HitResult, ShotDirection);
	if(bSuccess)
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		
		AActor* DamagedActor = HitResult.GetActor();
		if(DamagedActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController(); // 두번 계산하더라도, 코드의 가독성이 좋아지고 동기화가 어긋날 수 있는 변수나 상태를 저장하지 않아도 된다
			DamagedActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

	// DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Red, true);
	UE_LOG(LogTemp, Warning, TEXT("You Pull the Trigger!"));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController =  GetOwnerController();
	if(OwnerController == nullptr)
		return false;
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location,Rotation);
	ShotDirection = -Rotation.Vector();

	// 끝점을 위한 FVector
	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) nullptr;
	return OwnerPawn->GetController();
}
