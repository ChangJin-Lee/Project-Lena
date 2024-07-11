// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/Public/Items/Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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
	if(IsEmpty())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AmmoEmptySound, GetActorLocation());
		return;
	}
	
	if (MuzzleFlashNS)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashNS,
			Mesh,
			TEXT("MuzzleFlashSocket"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale,
			true
		);
	}
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult HitResult;
	FVector ShotDirection;

	bool bSuccess = GunTrace(HitResult, ShotDirection);
	if(bSuccess)
	{
		// DrawDebugPoint(GetWorld(), HitResult.Location, 15, FColor::Red, false, 5);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);

		AActor* DamagedActor = HitResult.GetActor();
		if(DamagedActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController(); // 두번 계산하더라도, 코드의 가독성이 좋아지고 동기화가 어긋날 수 있는 변수나 상태를 저장하지 않아도 된다
			DamagedActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			Ammo--;
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("You Pull the Trigger!"));
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

void AGun::SetAmmo(int AmmoCount)
{
	Ammo=AmmoCount;
}

int AGun::GetAmmo()
{
	return Ammo;
}

bool AGun::IsEmpty()
{
	return Ammo <= 0;
}

int AGun::GetMaxAmmo()
{
	return MaxAmmo;
}
