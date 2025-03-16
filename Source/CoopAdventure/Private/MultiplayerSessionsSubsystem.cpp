
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	onlineSubsystem = Online::GetSubsystem(UObject::GetWorld());
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
		}
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

//this is fired when we call create session from CreateServer function
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	PrintString("Session Created successful");
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/thirdPerson/maps/ThirdPersonMap?listen");
	}
}

void UMultiplayerSessionsSubsystem::PrintString(const FString& String)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
}

// UFUNCTION
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

// UFUNCTION
void UMultiplayerSessionsSubsystem::FindServer(const FString& serverName)
{
	PrintString("Finding Server");
}
