
#include "Transporter.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	moveTime = 3.f;
	activatedTriggerCount = 0;

	startPoint = FVector::Zero();
	endPoint = FVector::Zero();
	bArePointsSet = false;
}

void UTransporter::BeginPlay()
{
	Super::BeginPlay();

}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTransporter::SetPoints(FVector point1, FVector point2)
{
	if (point1.Equals(point2)) return; 

	startPoint = point1;
	endPoint = point2;
	bArePointsSet = true;
}

