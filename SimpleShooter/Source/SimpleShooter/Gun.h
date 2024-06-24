// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	void PullTrigger();

	UFUNCTION(BlueprintCallable)
	void SetAmmo(int AmmoCount);

	UFUNCTION(BlueprintCallable)
	int GetAmmo();

	int GetMaxAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* MuzzleFlashNS;  // Niagara 시스템 변수 추가

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AmmoEmptySound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	float MaxRange = 2000;

	UPROPERTY(EditAnywhere)
	float Damage = 15;

	UPROPERTY(EditAnywhere)
	int MaxAmmo = 30;
	
	UPROPERTY(EditAnywhere)
	int Ammo = 0;

	bool GunTrace(FHitResult &Hit, FVector &ShotDirection);

	AController* GetOwnerController() const;

	bool IsEmpty();
};
