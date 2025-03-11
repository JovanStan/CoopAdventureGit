
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//return online subsystem
	IOnlineSubsystem* onlineSubsystem = Online::GetSubsystem(GetWorld());
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
