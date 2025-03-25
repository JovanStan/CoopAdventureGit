
#include "MovableActor.h"

#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"

AMovableActor::AMovableActor()
{

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mesh->SetupAttachment(root);
	mesh->SetIsReplicated(true);

	point1 = CreateDefaultSubobject<UArrowComponent>("Point1");
	point1->SetupAttachment(root);
	point1->SetRelativeLocation(FVector::Zero());

	point2 = CreateDefaultSubobject<UArrowComponent>("Point2");
	point2->SetupAttachment(root);
	point2->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	transporter = CreateDefaultSubobject<UTransporter>("Transporter");

	box = CreateDefaultSubobject<UBoxComponent>("Box");
	box->SetupAttachment(root);
	box->SetCollisionResponseToChannels(ECR_Overlap);
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	FVector startPoint = GetActorLocation() + point1->GetRelativeLocation();
	FVector endPoint = GetActorLocation() + point2->GetRelativeLocation();
	transporter->SetPoints(startPoint, endPoint);

	box->OnComponentBeginOverlap.AddDynamic(this, &AMovableActor::OnBoxBeginOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &AMovableActor::OnBoxEndOverlap);
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovableActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMovableActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

