
#include "MultiplayerSessionsSubsystem.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	PrintString("Constructor");
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PrintString("Initialization");
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
	PrintString("Deinitialization");
}

void UMultiplayerSessionsSubsystem::PrintString(const FString& String)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, String);
	}
}
