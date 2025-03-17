
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnDeactivated);

UCLASS()
class COOPADVENTURE_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	APressurePlate();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnActivated OnActivated;
	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnDeactivated OnDeactivated;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* triggerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsActivated;


};
