// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "GameFramework/Character.h"
#include "Base_Character.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABase_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* FindMeshByName(FName MeshName);

	void ToggleIsArmed();
	void Shoot();
	void ChangeWeapon1();
	void ChangeWeapon2();
	void HideAllWeapons();
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const; // 이 함수를 호출했다고 캐릭터의 어떠한 상태도 바뀌면 안되기 때문. 안전하게 const로 선언

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	FString OverLayString;

	UFUNCTION(BlueprintCallable, Category = "OverLay Slot")
	void SetOverLayString(FString st);

	UFUNCTION(BlueprintCallable, Category = "OverLay Slot")
	void AdjustOverLay();

	UFUNCTION(BlueprintCallable)
	bool GetIsAICharacter();

	UFUNCTION(BlueprintImplementableEvent, Category = "OverLay Slot")
	void DrawAIWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "OverLay Slot")
	void PutDownAIWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation Slot")
	void OnDead();
	
private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	
	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TArray<TSubclassOf<AGun>> GunClassArray;

	UPROPERTY()
	TArray<AGun*> GunArray;

	int WeaponActiveIndex = 0;
	
	bool IsAICharacter = false;

	bool IsArmed = false;
};