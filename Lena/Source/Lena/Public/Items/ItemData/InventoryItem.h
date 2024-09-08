#pragma once

#include <ShlObj.h>
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "InventoryItem.generated.h"

UENUM(BlueprintType)
enum class EInventorySlotType : uint8
{
	ISt_Inventory UMETA(DisplayName = "Inventory"),
	ISt_Equipment UMETA(DisplayName = "Equipment"),
	ISt_Ground UMETA(DisplayName = "Ground")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	It_None UMETA(DisplayName = "None"),
	It_Equipment UMETA(DisplayName = "Equipment"),
	It_Consumable UMETA(DisplayName = "Consumable"),
	It_Ammo UMETA(DisplayName = "Ammo"),
	It_Miscellaneous UMETA(DisplayName = "Miscellaneous")
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Et_None UMETA(DisplayName = "None"), // 비장비 아이템을 위한 기본 값
	Et_Weapon UMETA(DisplayName = "Weapon"),
	Et_Armor UMETA(DisplayName = "Armor"),
	Et_Helmet UMETA(DisplayName = "Helmet"),
	Et_Pants UMETA(DisplayName = "Pants"),
	Et_Shoes UMETA(DisplayName = "Shoes"),
	Et_Shield UMETA(DisplayName = "Shield")
};

UENUM(BlueprintType)
enum class EGunType : uint8
{
	Gt_None UMETA(DisplayName = "None"),
	Gt_AssaultRifle UMETA(DisplayName = "AssaultRifle"),
	Gt_SniperRifle UMETA(DisplayName = "SniperRifle"),
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	At_None UMETA(DisplayName = "None"),
	At_556MM UMETA(DisplayName = "5.56mm"),
	At_762MM UMETA(DisplayName = "7.62mm"),
	At_9MM UMETA(DisplayName = "9mm")
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<AActor> ItemClass; // 아이템 클래스 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* ItemImage; // 아이템 이미지 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UMaterialInterface* ItemImageMaterial; // 아이템 이미지 Material 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemDescription; // 추가 정보

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item")
	AActor* ItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType; // 아이템 타입 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EGunType GunType; // 아이템 타입 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EEquipmentType EquipmentType; // 장비 타입 추가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 MaxAmmoCapacity; // 최대 장전 가능 탄약 수

	FInventoryItem()
		: ItemID(TEXT("")), ItemName(TEXT("")), ItemClass(nullptr), ItemImage(nullptr), Quantity(1), Weight(0), ItemDescription(TEXT("")), ItemActor(nullptr), ItemType(EItemType::It_None), GunType(EGunType::Gt_None), EquipmentType(EEquipmentType::Et_None), AmmoType(EAmmoType::At_None), MaxAmmoCapacity(0)
	{}

	bool IsEmpty() const
	{
		return ItemID.IsEmpty() && ItemName.IsEmpty() && ItemClass == nullptr && ItemImage == nullptr && ItemImageMaterial == nullptr && Quantity == 0 && Weight == 0.0f && ItemDescription.IsEmpty() && ItemActor == nullptr && ItemType == EItemType::It_None && GunType == EGunType::Gt_None && EquipmentType == EEquipmentType::Et_None && AmmoType == EAmmoType::At_None;
	}

	void Empty()
	{
		ItemID.Empty();
		ItemName.Empty();
		ItemClass = nullptr;
		ItemImage = nullptr;
		ItemImageMaterial = nullptr;
		Quantity = 0;
		Weight = 0.0f;
		ItemDescription.Empty();
		ItemActor = nullptr;
		GunType = EGunType::Gt_None;
		ItemType = EItemType::It_None;
		EquipmentType = EEquipmentType::Et_None;
		AmmoType = EAmmoType::At_None;
	}
};
