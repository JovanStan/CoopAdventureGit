
#include "CollectableKey.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	
	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetupAttachment(root);
	mesh->SetIsReplicated(true);
	mesh->SetCollisionResponseToChannels(ECR_Overlap);

	capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	capsule->SetupAttachment(root);
	capsule->SetIsReplicated(true);
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

	//Get reference to the player 
	playerCharacter = Cast<ACoopAdventureCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ACoopAdventureCharacter::StaticClass()));
	
}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		mesh->AddRelativeRotation(FRotator(0.f, rotateSpeed * DeltaTime, 0.f));
	}
}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ACollectableKey, bIsCollected);
}

void ACollectableKey::OnRep_IsCollected()
{
	collectSound->Play();
	
	if (HasAuthority())
	{
		mesh->SetVisibility(false);
		Multicast_UpdateUI();
	}
}

void ACollectableKey::Multicast_UpdateUI_Implementation()
{
	OnUpdateUI.Broadcast();
}


void ACollectableKey::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (!bIsCollected && OtherActor->ActorHasTag("Player"))
		{
			bIsCollected = true;
			OnRep_IsCollected();
		}
	}
}




