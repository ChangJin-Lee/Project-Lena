// Fill out your copyright notice in the Description page of Project Settings.

#include "Base_Character.h"
#include "SimpleShooterGameMode.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if(!ensure(!GunClassArray.IsEmpty())) return;
	
	for(TSubclassOf<AGun> GunClass : GunClassArray)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		Gun->SetActorHiddenInGame(true);
		GunArray.Add(Gun);
	}
	
	// GunArray[WeaponActiveIndex]->SetActorHiddenInGame(false);
}

// Called every frame
void ABase_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &ABase_Character::Shoot);
	PlayerInputComponent->BindAction(TEXT("DrawWeapon1"), EInputEvent::IE_Pressed, this, &ABase_Character::ChangeWeapon1);
	PlayerInputComponent->BindAction(TEXT("DrawWeapon2"), EInputEvent::IE_Pressed, this, &ABase_Character::ChangeWeapon2);
}


void ABase_Character::Shoot()
{
	if(IsArmed)
	{
		GunArray[WeaponActiveIndex]->PullTrigger();
	}
	UE_LOG(LogTemp, Warning, TEXT("You Pull the Trigger!"));
}


float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;
	
	UE_LOG(LogTemp, Warning, TEXT("Health Left : %f"), Health);

	if(IsDead())
	{
		ASimpleShooterGameMode *GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameMode>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OnDead();
	}
	
	return DamageToApplied;
}

bool ABase_Character::IsDead() const
{
	// 체력이 0보다 작으면 true를 반환.
	return Health <= 0;
}

float ABase_Character::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void ABase_Character::ChangeWeapon1()
{
	WeaponActiveIndex = 0;
	ChangeWeapon();
}

void ABase_Character::ChangeWeapon2()
{
	WeaponActiveIndex = 1;
	ChangeWeapon();
}

void ABase_Character::ChangeWeapon()
{
	for(int32 WeaponIndex = 0 ; WeaponIndex < GunArray.Num(); WeaponIndex++)
	{
		if(WeaponIndex == WeaponActiveIndex)
		{
			GunArray[WeaponIndex]->SetActorHiddenInGame(false);
		}
		else
		{
			GunArray[WeaponIndex]->SetActorHiddenInGame(true);
		}
	}
}

void ABase_Character::HideAllWeapons()
{
	for(int32 WeaponIndex = 0 ; WeaponIndex < GunArray.Num(); WeaponIndex++)
	{
		GunArray[WeaponIndex]->SetActorHiddenInGame(true);
	}
}

void ABase_Character::SetOverLayString(FString st)
{
	OverLayString = st;
	UE_LOG(LogTemp, Warning, TEXT("You Select Overlay : %s"), *OverLayString);
}

void ABase_Character::AdjustOverLay()
{
	// UE_LOG(LogTemp, Warning, TEXT("AdjustOverLay : %d"), WeaponActiveIndex);
	if(OverLayString == "Rifle")
	{
		ToggleIsArmed();
		ChangeWeapon();
	}
	else
	{
		ToggleIsArmed();
		HideAllWeapons();
	}
}

bool ABase_Character::GetIsAICharacter()
{
	return IsAICharacter;
}

void ABase_Character::ToggleIsArmed()
{
	IsArmed = !IsArmed;
}