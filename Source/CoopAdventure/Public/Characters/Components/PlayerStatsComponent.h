
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float maxHealth = 100.f;

public:
	FORCEINLINE float GetCurrentHealth() const { return currentHealth; }
	FORCEINLINE void SetCurrentHealth(float health) { currentHealth = health; }
	
	FORCEINLINE float GetMaxHealth() const { return maxHealth; }
	FORCEINLINE void SetMaxHealth(float health) { maxHealth = health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float SetProgressBarPercent();
	
};
