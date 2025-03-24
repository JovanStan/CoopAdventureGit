
#include "CollectableKey.h"

#include "Components/AudioComponent.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	
	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetupAttachment(root);
	mesh->SetCollisionResponseToChannels(ECR_Overlap);

	capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	capsule->SetupAttachment(root);
	capsule->SetCollisionResponseToChannels(ECR_Overlap);
	capsule->SetCapsuleHalfHeight(150.f);
	capsule->SetCapsuleRadius(100.f);

	rotateSpeed = 100.f;
	collectSound = CreateDefaultSubobject<UAudioComponent>("Collect Sound");
	collectSound->SetupAttachment(root);
	collectSound->SetAutoActivate(false);
	
}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	capsule->OnComponentBeginOverlap.AddDynamic(this, &ACollectableKey::OnCapsuleBeginOverlap);
}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mesh->AddRelativeRotation(FRotator(0.f, rotateSpeed * DeltaTime, 0.f));
}


void ACollectableKey::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsCollected && OtherActor->ActorHasTag("Player"))
	{
		bIsCollected = true;
		collectSound->Play();
		mesh->SetVisibility(false);
		OnUpdateUI.Broadcast();
	}
}




