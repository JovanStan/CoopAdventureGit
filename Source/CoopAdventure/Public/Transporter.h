
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	FVector startPoint;
	FVector endPoint;
	bool bArePointsSet;

	UPROPERTY(EditAnywhere)
	float moveTime;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> triggerActors;

	UPROPERTY(VisibleAnywhere)
	int activatedTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool bAllTriggerActorsTriggered;

	UFUNCTION()
	void SetPoints(FVector point1, FVector point2);
};
