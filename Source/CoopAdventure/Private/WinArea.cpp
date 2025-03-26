
#include "WinArea.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"

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

	if (!bWinCondition)
	{
		TArray<AActor*> overlappingActors;
		winAreaBox->GetOverlappingActors(overlappingActors, APlayerCharacter::StaticClass());

		bWinCondition = (overlappingActors.Num() == 2);
		if (bWinCondition)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "WinCondition");
			OnWinCondition.Broadcast();
		}
	}
}



