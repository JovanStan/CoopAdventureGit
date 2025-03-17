
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

	/*auto meshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("Game/Stylized_Egypt/Meshes/building/SM_block_02.SM_block_02"));
	if (meshAsset.Succeeded())
	{
		mesh->SetStaticMesh(meshAsset.Object);
		mesh->SetRelativeScale3D(FVector(4.f, 4.f, 0.5f));
		mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}*/
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

}

