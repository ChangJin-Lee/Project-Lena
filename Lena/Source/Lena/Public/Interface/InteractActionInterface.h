#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractActionInterface.generated.h"

UINTERFACE()
class UInteractActionInterface : public UInterface
{
	GENERATED_BODY()
};

class LENA_API IInteractActionInterface
{
	GENERATED_BODY()

public:

	virtual void Unlock() = 0;
	virtual void PerformAction() = 0;
};
