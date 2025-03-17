
#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, wasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoinDelegate, bool, wasSuccessful);

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

	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate serverCreateDelegate;

	UPROPERTY(BlueprintAssignable)
	FServerJoinDelegate serverJoinDelegate;

public:
	IOnlineSessionPtr sessionInterface;
	IOnlineSubsystem* onlineSubsystem;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	bool createServerAfterDestroyed;
	FString destroyServerName;
	FString serverNameToFind;
	FName sessionName;

	UPROPERTY(BlueprintReadWrite)
	FString mapPath;
};
