
#include "WinArea.h"

#include "Components/BoxComponent.h"
#include "CoopAdventure/CoopAdventureCharacter.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	winAreaBox = CreateDefaultSubobject<UBoxComponent>("Box Component");
	SetRootComponent(winAreaBox);

	bWinCondition = false;
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!bWinCondition)
		{
			TArray<AActor*> overlappingActors;
			winAreaBox->GetOverlappingActors(overlappingActors, ACoopAdventureCharacter::StaticClass());

			bWinCondition = (overlappingActors.Num() == 2);
			if (bWinCondition)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "WinCondition");
				MulticastRPC_Win();
			}
		}
	}
}

void AWinArea::MulticastRPC_Win_Implementation()
{
	OnWinCondition.Broadcast();
}

