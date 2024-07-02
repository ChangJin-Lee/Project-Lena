#pragma once

#include "CoreMinimal.h"
#include "Lena/Public/InteractableThings/InteractableActor.h"
#include "GameFramework/Actor.h"
#include "LockActor.generated.h"

UCLASS()
class LENA_API ALockActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	ALockActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
// protected:
	// UPROPERTY(VisibleAnywhere)
	// UStaticMeshComponent* BaseStaticMeshComponent;
	
	UPROPERTY(EditAnywhere)
	USoundBase* RightAnswerSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* WrongAnswerSound;
	
};