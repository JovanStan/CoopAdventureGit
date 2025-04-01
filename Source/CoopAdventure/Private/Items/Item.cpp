
#include "Items/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetupAttachment(root);
		
	sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	sphere->SetupAttachment(root);
	sphere->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IInteractableInterface* interactable = Cast<IInteractableInterface>(OtherActor);
	if (interactable)
	{
		interactable->SetOverlappingItem(this);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player character entered");
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractableInterface* interactable = Cast<IInteractableInterface>(OtherActor);
	if (interactable)
	{
		interactable->SetOverlappingItem(nullptr);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player character left");
	}
}



