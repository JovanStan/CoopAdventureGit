
#pragma once

#include "CoreMinimal.h"
#include "BodySwapSaveGame.h"
#include "Engine/GameInstance.h"
#include "BodySwapGameInstance.generated.h"

UCLASS()
class COOPADVENTURE_API UBodySwapGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY()
	UBodySwapSaveGame* currentSaveGame;
	
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void NewGame();
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void LoadGame();

	static bool DoesSaveExist();

	// save functions
	UFUNCTION(BlueprintCallable)
	void SaveMouseSensitivity(float NewSensitivity);
	UFUNCTION(BlueprintCallable)
	void SaveInvertX(bool newInvertX);
	UFUNCTION(BlueprintCallable)
	void SaveInvertY(bool newInvertY);
	UFUNCTION(BlueprintCallable)
	void SaveMusicVolume(float newMusicVolume);
	UFUNCTION(BlueprintCallable)
	void SaveSfxVolume(float newSfxVolume);
	UFUNCTION(BlueprintCallable)
	void SaveVoiceVolume(float newVoiceVolume);

	// properties to save
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	float MouseSensitivity;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FVector PlayerLocation;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	bool bInvertX;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	bool bInvertY;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	float MusicVolume;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	float SfxVolume;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "SaveData")
	float VoiceVolume;

};


