// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// SetRootComponent(Root); RootComponent = Root;의 차이에 대해서 잘 알아두자.
	// RootComponent = Root;
	//
	// GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	// GunMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("selectedOverlay  : %d"), selectedOverlay);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ABaseCharacter::Shoot);
}


bool ABaseCharacter::IsDead() const
{
	// 체력이 0보다 작으면 true를 반환.
	return Health <= 0;
}

float ABaseCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void ABaseCharacter::Shoot()
{
	PullTrigger();// Shoot
}


// gun으로 옮겨야함.

void ABaseCharacter::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleFlashSocket"));
	
	FHitResult HitResult;
	FVector ShotDirection;

	bool bSuccess = GunTrace(HitResult, ShotDirection);
	if(bSuccess)
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
		
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


bool ABaseCharacter::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
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

AController* ABaseCharacter::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) nullptr;
	return OwnerPawn->GetController();
}