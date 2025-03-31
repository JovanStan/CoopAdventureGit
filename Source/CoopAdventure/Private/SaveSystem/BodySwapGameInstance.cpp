
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
	currentSaveGame->MusicVolume = MusicVolume;
	currentSaveGame->SfxVolume = SfxVolume;
	currentSaveGame->VoiceVolume = VoiceVolume;

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
			MusicVolume = currentSaveGame->MusicVolume;
			SfxVolume = currentSaveGame->SfxVolume;
			VoiceVolume = currentSaveGame->VoiceVolume;
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

void UBodySwapGameInstance::SaveMusicVolume(float newMusicVolume)
{
	if (!currentSaveGame) LoadGame();  // Load the existing save file if needed
	if (!currentSaveGame) return;  // Safety check

	MusicVolume = newMusicVolume;
	currentSaveGame->MusicVolume = MusicVolume;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::SaveSfxVolume(float newSfxVolume)
{
	if (!currentSaveGame) LoadGame();  // Load the existing save file if needed
	if (!currentSaveGame) return;  // Safety check

	SfxVolume = newSfxVolume;
	currentSaveGame->SfxVolume = SfxVolume;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

void UBodySwapGameInstance::SaveVoiceVolume(float newVoiceVolume)
{
	if (!currentSaveGame) LoadGame();  // Load the existing save file if needed
	if (!currentSaveGame) return;  // Safety check

	VoiceVolume = newVoiceVolume;
	currentSaveGame->VoiceVolume = VoiceVolume;

	UGameplayStatics::SaveGameToSlot(currentSaveGame, "Save", 0);
}

bool UBodySwapGameInstance::DoesSaveExist()
{
	return  UGameplayStatics::DoesSaveGameExist("Save", 0);
}
