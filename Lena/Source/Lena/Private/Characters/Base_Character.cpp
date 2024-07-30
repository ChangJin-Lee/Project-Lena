// Fill out your copyright notice in the Description page of Project Settings.

#include "Lena/Public/Characters/Base_Character.h"

#include "EnhancedInputSubsystems.h"
#include "Lena/Public/Game/GameMode/LenaGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Items/Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABase_Character::ABase_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	GroundItemsComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("GroundItemsComponent"));
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
			// SpecificMesh->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
			// Gun->AttachToComponent(SpecificMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LHS_ik_hand_r"));
			// Gun->SetOwner(this);
			// Gun->SetActorHiddenInGame(true);
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
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ABase_Character::ReloadAction);
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

void ABase_Character::ReloadAction()
{
	ReloadGun();
}

void ABase_Character::ReloadAIAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload Action Event!"));
	ReloadAIEvent();
	ReloadGun();
}

float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;
	
	UE_LOG(LogTemp, Warning, TEXT("Health Left : %f"), Health);

	if(IsDead())
	{
		ALenaGameMode *GameMode = GetWorld()->GetAuthGameMode<ALenaGameMode>();
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

void ABase_Character::AddAmmo_5mm()
{
	int MaxAmmo = GunArray[WeaponActiveIndex]->GetMaxAmmo();
	Ammo_5mm+=MaxAmmo;
}

AGun* ABase_Character::GetWeapon()
{
	return GunArray[WeaponActiveIndex];
}

bool ABase_Character::ReloadGun()
{
	int MaxAmmo = GunArray[WeaponActiveIndex]->GetMaxAmmo();
	int CurrentAmmo = GunArray[WeaponActiveIndex]->GetAmmo();

	if(!GunArray[WeaponActiveIndex])
		return false;

	if(CurrentAmmo < MaxAmmo)
	{
		GunArray[WeaponActiveIndex]->SetAmmo(FMath::Min(MaxAmmo, CurrentAmmo+Ammo_5mm));
		Ammo_5mm = FMath::Max( 0 , Ammo_5mm-(MaxAmmo - CurrentAmmo));
	}
	
	return true;
}

// -----------------------------
// Inventory
// -----------------------------

// 아이템 떨어뜨리기
void ABase_Character::DropItem(FInventoryItem ItemData)
{
	ABase_Item* Item = Cast<ABase_Item>(ItemData.ItemActor);
	// ItemActor에서 ItemID를 가져옵니다. ItemActor는 아이템 정보를 제공해야 합니다.

	if(Item)
	{
		float Rad = 20.0f; // 반경 설정
		float InnerRad = 5.0f; // 제외할 범위 설정

		// 랜덤한 값을 선택하는 함수
		auto GetRandomOffset = [Rad, InnerRad]() -> float
		{
			float Offset;
			do
			{
				Offset = FMath::RandRange(-Rad, Rad);
			} while (Offset > -InnerRad && Offset < InnerRad);
			return Offset;
		};

		float RandomX = GetRandomOffset();
		float RandomY = GetRandomOffset();
		
		FVector Location = GetActorLocation() + (RandomX, RandomY, 0.0f);
		FActorSpawnParameters Parameters;
		ABase_Item* SpawnedItem =  GetWorld()->SpawnActor<ABase_Item>(Item->GetClass(), Location, GetActorRotation(), Parameters);
		if(SpawnedItem)
		{
			SpawnedItem->ItemID = ItemData.ItemID;
			SpawnedItem->ItemName = ItemData.ItemName;
			SpawnedItem->ItemImage = ItemData.ItemImage;
			SpawnedItem->Quantity = ItemData.Quantity;
			SpawnedItem->weight = ItemData.weight;
			SpawnedItem->ItemDescription = ItemData.ItemDescription;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropItemSound, GetActorLocation());
		}
	}
}

// 아이템 줍기
void ABase_Character::PickupItem(AActor* ItemActor)
{
	if (ItemActor)
	{
		ABase_Item* Item = Cast<ABase_Item>(ItemActor);
		// ItemActor에서 ItemID를 가져옵니다. ItemActor는 아이템 정보를 제공해야 합니다.
		if(Item)
		{
			FInventoryItem InventoryItem;
			InventoryItem.ItemID = Item->ItemID;
			InventoryItem.ItemName = Item->ItemName;
			InventoryItem.ItemImage = Item->ItemImage;
			InventoryItem.Quantity = Item->Quantity;
			InventoryItem.weight = Item->weight;
			InventoryItem.ItemDescription = Item->ItemDescription;
			InventoryItem.ItemActor = Item;
			
			if (InventoryComponent)
			{
				InventoryComponent->AddItem(InventoryItem);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupItemSound, GetActorLocation());
			}
			
			// 아이템 액터를 월드에서 제거합니다.
			ItemActor->Destroy();
		}
	}
}

void ABase_Character::CheckGroundItem(AActor* ItemActor)
{
	if (ItemActor)
	{
		ABase_Item* Item = Cast<ABase_Item>(ItemActor);
		
		if(Item)
		{
			FInventoryItem InventoryItem;
			InventoryItem.ItemID = Item->ItemID;
			InventoryItem.ItemName = Item->ItemName;
			InventoryItem.ItemImage = Item->ItemImage;
			InventoryItem.Quantity = Item->Quantity;
			InventoryItem.weight = Item->weight;
			InventoryItem.ItemDescription = Item->ItemDescription;
			InventoryItem.ItemActor = Item;
			
			if (GroundItemsComponent)
			{
				GroundItemsComponent->AddItem(InventoryItem);
			}
		}
	}
}
