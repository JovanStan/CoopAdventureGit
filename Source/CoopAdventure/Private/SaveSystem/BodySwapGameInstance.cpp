
#include "SaveSystem/BodySwapGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/BodySwapSaveGame.h"

void UBodySwapGameInstance::Init()
{
	Super::Init();
	LoadGame();
}

void UBodySwapGameInstance::NewGame()
{
	currentSaveGame = Cast<UBodySwapSaveGame>(UGameplayStatics::CreateSaveGameObject(UBodySwapSaveGame::StaticClass()));

	if (currentSaveGame)
	{
		MouseSensitivity = 1.f;
		PlayerLocation = FVector::ZeroVector;
		bInvertX = false;
		bInvertY = false;
		
		SaveGame();
	}
}

void UBodySwapGameInstance::SaveGame()
{
	if (!currentSaveGame)
	{
		currentSaveGame = Cast<UBodySwapSaveGame>(UGameplayStatics::CreateSaveGameObject(UBodySwapSaveGame::StaticClass()));
	}

	if (!currentSaveGame) return;

	currentSaveGame->MouseSensitivity = MouseSensitivity;
	currentSaveGame->bInvertX = bInvertX;
	currentSaveGame->bInvertY = bInvertY;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::LoadGame()
{
	if (!DoesSaveExist()) return;

	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot("Save", 0);
	if (LoadedGame)
	{
		currentSaveGame = Cast<UBodySwapSaveGame>(LoadedGame);

		if (currentSaveGame)
		{
			// Load settings from the saved file
			MouseSensitivity = currentSaveGame->MouseSensitivity;
			bInvertX = currentSaveGame->bInvertX;
			bInvertY = currentSaveGame->bInvertY;
		}
	}
}

void UBodySwapGameInstance::SaveMouseSensitivity(float NewSensitivity)
{
	if (!currentSaveGame) LoadGame();  // Load the existing save file if needed
	if (!currentSaveGame) return;  // Safety check

	MouseSensitivity = NewSensitivity;
	currentSaveGame->MouseSensitivity = MouseSensitivity;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::SaveInvertX(bool newInvertX)
{
	if (!currentSaveGame) LoadGame(); 
	if (!currentSaveGame) return;
	
	bInvertX = newInvertX;
	currentSaveGame->bInvertX = bInvertX;
	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::SaveInvertY(bool newInvertY)
{
	if (!currentSaveGame) LoadGame(); 
	if (!currentSaveGame) return;

	bInvertY = newInvertY;
	currentSaveGame->bInvertY = bInvertY;
	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

bool UBodySwapGameInstance::DoesSaveExist()
{
	return  UGameplayStatics::DoesSaveGameExist("Save", 0);
}
