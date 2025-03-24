
#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "CollectableKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateUI);

UCLASS()
class COOPADVENTURE_API ACollectableKey : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectableKey();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateUI OnUpdateUI;

private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess=true))
	USceneComponent* root;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess=true))
	UCapsuleComponent* capsule;

	
	UPROPERTY(ReplicatedUsing= OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere, meta=(AllowPrivateAccess=true))
	bool bIsCollected;

	UFUNCTION()
	void OnRep_IsCollected();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess=true))
	float rotateSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess=true))
	UAudioComponent* collectSound;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateUI();

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	APlayerCharacter* playerCharacter;

public:
	FORCEINLINE bool GetIsCollected() const { return bIsCollected; }
	
};

