
#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

UCLASS()
class COOPADVENTURE_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();
	void static PrintString(const FString& String);

	UFUNCTION(BlueprintCallable)
	void CreateServer(const FString& serverName);
	UFUNCTION(BlueprintCallable)
	void FindServer(const FString& serverName);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//this function is bind to OnCreateSessionCompleteDelegates
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	//this function is bind to OnDestroySessionCompleteDelegates
	void OnDestroySessionComplete(FName sessionName, bool bWasSuccessful);

public:
	IOnlineSessionPtr sessionInterface;
	IOnlineSubsystem* onlineSubsystem;

	bool createServerAfterDestroyed;
	FString destroyServerName;
};
