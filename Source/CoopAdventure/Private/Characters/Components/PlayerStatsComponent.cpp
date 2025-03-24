
#include "Characters/Components/PlayerStatsComponent.h"


UPlayerStatsComponent::UPlayerStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	currentHealth = maxHealth;
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UPlayerStatsComponent::SetProgressBarPercent()
{
	return currentHealth / maxHealth;
}

void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

