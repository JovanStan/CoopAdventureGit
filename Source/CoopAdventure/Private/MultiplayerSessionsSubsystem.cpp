
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	onlineSubsystem = Online::GetSubsystem(UObject::GetWorld());

	createServerAfterDestroyed = false;
	serverNameToFind = "";
	sessionName = FName("Coop Adventure");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//return online subsystem
	if (onlineSubsystem)
	{
		//get session interface from onlineSubsystem (steam)
		sessionInterface = onlineSubsystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

//this is fired when we call sessionInterface->CreateSession(0, sessionName, sessionSettings); in CreateServer function
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName _sessionName, bool bWasSuccessful)
{
	PrintString("Session Created successful");
	serverCreateDelegate.Broadcast(bWasSuccessful);
	if (bWasSuccessful)
	{
		FString path = "/Game/ThirdPerson/Maps/ThirdPersonMap?listen";
		if (!mapPath.IsEmpty())
		{
			path = FString::Printf(TEXT("%s?listen"), *mapPath);
		}
		GetWorld()->ServerTravel(path);
	}
}

// this function is fired when we call sessionInterface->DestroySession(sessionName); in CreateServer
void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName _sessionName, bool bWasSuccessful)
{
	if (createServerAfterDestroyed)
	{
		createServerAfterDestroyed = false;
		CreateServer(destroyServerName);
	}
}

// UFUNCTION
void UMultiplayerSessionsSubsystem::CreateServer(const FString& serverName)
{
	PrintString("Creating Server");
	if (serverName.IsEmpty())
	{
		PrintString("Server Name is Empty");
		serverCreateDelegate.Broadcast(false);
		return;
	}

	FNamedOnlineSession* existingSession = sessionInterface->GetNamedSession(sessionName);
	if (existingSession)
	{
		createServerAfterDestroyed = true;
		destroyServerName = serverName;
		sessionInterface->DestroySession(sessionName);
		return;
	}
	
	FOnlineSessionSettings sessionSettings;
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.NumPublicConnections = 2;
	sessionSettings.bUseLobbiesIfAvailable = true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.bAllowJoinViaPresence = true;

	bool bIsLan = false;
	if (onlineSubsystem->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}
	sessionSettings.bIsLANMatch = bIsLan;
	sessionSettings.Set(FName("SERVER_NAME"), serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	sessionInterface->CreateSession(0, sessionName, sessionSettings);
}

// UFUNCTION
void UMultiplayerSessionsSubsystem::FindServer(const FString& serverName)
{
	PrintString("Finding Server");
	
	if (serverName.IsEmpty())
	{
		PrintString("Server Name is Empty");
		serverJoinDelegate.Broadcast(false);
		return;
	}

	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool bIsLan = false;
	if (onlineSubsystem->GetSubsystemName() == "NULL")
	{
		bIsLan = true;
	}
	sessionSearch->bIsLanQuery = bIsLan;
	sessionSearch->MaxSearchResults = 9999;
	sessionSearch->QuerySettings.Set(FName("SEARCH_PRESENCE"), true, EOnlineComparisonOp::Equals);

	serverNameToFind = serverName;
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	if (serverNameToFind.IsEmpty()) return;
	
	TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;
	FOnlineSessionSearchResult* correctResult = 0;
	
	if (results.Num() > 0)
	{
		for (FOnlineSessionSearchResult result : results)
		{
			if (result.IsValid())
			{
				FString serverName = "No-name";
				result.Session.SessionSettings.Get(FName("SERVER_NAME"), serverName);
				PrintString(serverName);

				if (serverName.Equals(serverNameToFind))
				{
					correctResult = &result;
					break;
				}
			}
		}
		if (correctResult)
		{
			sessionInterface->JoinSession(0, sessionName, *correctResult);
		}else
		{
			serverJoinDelegate.Broadcast(false);
		}
	}else
	{
		serverJoinDelegate.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName _sessionName, EOnJoinSessionCompleteResult::Type result)
{
	serverJoinDelegate.Broadcast(result == EOnJoinSessionCompleteResult::Success);
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		FString address = "";
		bool success = sessionInterface->GetResolvedConnectString(_sessionName, address);
		if (success)
		{
			APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (playerController)
			{
				playerController->ClientTravel(address, TRAVEL_Absolute);
			}
		}
	}
}

void UMultiplayerSessionsSubsystem::PrintString(const FString& String)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
}
