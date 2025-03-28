
#include "SaveSystem/BodySwapGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveSystem/BodySwapSaveGame.h"

void UBodySwapGameInstance::Init()
{
	Super::Init();
}

void UBodySwapGameInstance::NewGame()
{
	currentSaveGame = Cast<UBodySwapSaveGame>(UGameplayStatics::CreateSaveGameObject(UBodySwapSaveGame::StaticClass()));

	if (currentSaveGame)
	{
		MouseSensitivity = 1.f;
		PlayerLocation = FVector::ZeroVector;

		SaveGame();
	}
}

void UBodySwapGameInstance::SaveGame()
{
	if (!currentSaveGame) return;
	
	//currentSaveGame->MouseSensitivity = MouseSensitivity;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::LoadGame()
{
	if (!DoesSaveExist()) return;

	currentSaveGame = Cast<UBodySwapSaveGame>(UGameplayStatics::LoadGameFromSlot("Save", 0));

	if (currentSaveGame)
	{
		MouseSensitivity = currentSaveGame->MouseSensitivity;
	}
}

void UBodySwapGameInstance::SaveMouseSensitivity(float NewSensitivity)
{
	if (!currentSaveGame) return;
	
	MouseSensitivity = NewSensitivity; 
	currentSaveGame->MouseSensitivity = NewSensitivity;
	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

bool UBodySwapGameInstance::DoesSaveExist()
{
	return  UGameplayStatics::DoesSaveGameExist("Save", 0);
}
