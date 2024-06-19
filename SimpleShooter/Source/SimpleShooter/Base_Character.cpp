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

	USkeletalMeshComponent* SpecificMesh = FindMeshByName(TEXT("BodyMesh"));
	if (SpecificMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find  VRoidCharacter  %s"), *SpecificMesh->GetName());
		
		// 총 부착
		for(TSubclassOf<AGun> GunClass : GunClassArray)
		{
			Gun = GetWorld()->SpawnActor<AGun>(GunClass);
			SpecificMesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
			Gun->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Gun->SetOwner(this);
			Gun->SetActorHiddenInGame(true);
			GunArray.Add(Gun);
			UE_LOG(LogTemp, Warning, TEXT("Spawn Guns  %s"), *Gun->GetName());
		}
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

// 특정 이름을 가진 자식 메쉬를 찾는 함수
USkeletalMeshComponent* ABase_Character::FindMeshByName(FName MeshName)
{
	// 모든 SkeletalMeshComponent를 찾습니다.
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);

	// 각 컴포넌트를 순회하며 이름이 일치하는 메쉬를 찾습니다.
	for (USkeletalMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp->GetName() == MeshName.ToString())
		{
			return MeshComp;
		}
	}

	// 찾지 못하면 nullptr 반환
	return nullptr;
}


void ABase_Character::Shoot()
{
	if(IsArmed)
	{ 
		GunArray[WeaponActiveIndex]->PullTrigger();
	}
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
}

void ABase_Character::AdjustOverLay()
{
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

int ABase_Character::GetAmmo()
{
	return GunArray[WeaponActiveIndex]->GetAmmo();
}

void ABase_Character::AddAmmo(int mount)
{
	return GunArray[WeaponActiveIndex]->AddAmmo(mount);
}

AGun* ABase_Character::GetWeapon()
{
	return GunArray[WeaponActiveIndex];
}
