
#include "PressurePlate.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsActivated = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	triggerMesh = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
	triggerMesh->SetupAttachment(root);
	triggerMesh->SetIsReplicated(true);
	

	/*auto triggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (triggerMeshAsset.Succeeded())
	{
		triggerMesh->SetStaticMesh(triggerMeshAsset.Object);
		triggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		triggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}*/

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetupAttachment(root);
	mesh->SetIsReplicated(true);

}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	triggerMesh->SetVisibility(false);
	triggerMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}

void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		AActor* overlappingActor = 0;
		
		TArray<AActor*> overlappingActors;
		triggerMesh->GetOverlappingActors(overlappingActors);

		for (int i = 0; i < overlappingActors.Num(); i++)
		{
			if (overlappingActors[i]->ActorHasTag("Player"))
			{
				overlappingActor = overlappingActors[i];
				break;
			}
		}
		if (overlappingActor)
		{
			if (!bIsActivated)
			{
				bIsActivated = true;
				OnActivated.Broadcast();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Activated");
			}
		}else
		{
			if (bIsActivated)
			{
				bIsActivated = false;
				OnDeactivated.Broadcast();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Not Activated");
			}
		}
	}
}

