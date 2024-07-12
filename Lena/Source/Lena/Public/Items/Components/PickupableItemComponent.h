#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupableItemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYGAME_API UPickupableItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:    
	UPickupableItemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity;
};
