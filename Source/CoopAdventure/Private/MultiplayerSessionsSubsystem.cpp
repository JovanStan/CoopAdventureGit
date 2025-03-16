
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//return online subsystem
	onlineSubsystem = Online::GetSubsystem(GetWorld());
	if (onlineSubsystem)
	{
		PrintString(onlineSubsystem->GetSubsystemName().ToString());

		//get session interface from onlineSubsystem (steam)
		sessionInterface = onlineSubsystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			PrintString("Session Interface is Valid");
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMultiplayerSessionsSubsystem::PrintString(const FString& String)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
}

void UMultiplayerSessionsSubsystem::CreateServer(const FString& serverName)
{
	PrintString("Creating Server");
	if (serverName.IsEmpty())
	{
		PrintString("Server Name is Empty");
		return;
	}

	FName sessionName = FName("Coop Adventure");
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
	
	sessionInterface->CreateSession(0, sessionName, sessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(const FString& serverName)
{
	PrintString("Finding Server");
}
