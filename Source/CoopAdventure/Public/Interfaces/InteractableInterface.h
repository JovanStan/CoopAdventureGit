
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class COOPADVENTURE_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(class AItem* item);
};
