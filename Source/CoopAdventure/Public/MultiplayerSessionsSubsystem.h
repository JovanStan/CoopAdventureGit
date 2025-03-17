
#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
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
	void OnCreateSessionComplete(FName _sessionName, bool bWasSuccessful);

	//this function is bind to OnDestroySessionCompleteDelegates
	void OnDestroySessionComplete(FName _sessionName, bool bWasSuccessful);

	//this function is bind to OnFindSessionsCompleteDelegates
	void OnFindSessionsComplete(bool bWasSuccessful);

	//this function is bind to OnJoinSessionsCompleteDelegates
	void OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type result);

public:
	IOnlineSessionPtr sessionInterface;
	IOnlineSubsystem* onlineSubsystem;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	bool createServerAfterDestroyed;
	FString destroyServerName;
	FString serverNameToFind;
	FName sessionName;
};
