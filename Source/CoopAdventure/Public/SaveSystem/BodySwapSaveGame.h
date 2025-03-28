
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BodySwapSaveGame.generated.h"

UCLASS()
class COOPADVENTURE_API UBodySwapSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UBodySwapSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float MouseSensitivity;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	bool bInvertX;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	bool bInvertY;
};
