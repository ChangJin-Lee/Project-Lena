// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadWrite, Category="OverlaySwitch")
	int selectedOverlay = -1;

	UFUNCTION(BlueprintPure)
	bool IsDead() const; // 이 함수를 호출했다고 캐릭터의 어떠한 상태도 바뀌면 안되기 때문. 안전하게 const로 선엄

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	
	void Shoot();
	
	// Gun으로 분리해야하는 함수 및 변수들
	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);
	AController* GetOwnerController() const;
	void PullTrigger();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 15;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;
	
};
