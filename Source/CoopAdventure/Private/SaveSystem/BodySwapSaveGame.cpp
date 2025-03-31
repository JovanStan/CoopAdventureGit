
#include "SaveSystem/BodySwapSaveGame.h"

UBodySwapSaveGame::UBodySwapSaveGame()
{
	MouseSensitivity = 1.f;
	PlayerLocation = FVector::ZeroVector;
	bInvertX = false;
	bInvertY = false;
	MusicVolume = .5f;
	SfxVolume = .5f;
	VoiceVolume = .5f;
}
