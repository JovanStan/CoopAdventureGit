
#include "Transporter.h"

#include "PressurePlate.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	moveTime = 3.f;

	startPoint = FVector::Zero();
	endPoint = FVector::Zero();
	bArePointsSet = false;
}

void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	APressurePlate* pressurePlate = Cast<APressurePlate>(triggerActor);
	if (pressurePlate)
	{
		pressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnPressurePlateActivated);
		pressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnPressurePlateDeactivated);
	}
}

void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTransporter::OnPressurePlateActivated()
{
	GEngine->AddOnScreenDebugMessage(-1, moveTime, FColor::Red, "Pressure Plate activated");
}

void UTransporter::OnPressurePlateDeactivated()
{
	GEngine->AddOnScreenDebugMessage(-1, moveTime, FColor::Red, "Pressure Plate deactivated");
}

void UTransporter::SetPoints(FVector point1, FVector point2)
{
	if (point1.Equals(point2)) return; 

	startPoint = point1;
	endPoint = point2;
	bArePointsSet = true;
}

