
#pragma once

#include "CoreMinimal.h"
#include "CollectableKey.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class COOPADVENTURE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprinting();
	void StopSprinting();

	
private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Mapping Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCameraViewAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeCharacterAction;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACollectableKey* CollectableKey;
	
	//springting
	bool bIsSprinting;
	float StartArmLength;
	float TargetArmLength = 500.f;
	float ArmLengthInterpSpeed = 1.5f;

	bool bIsFirstPerson;
	void ToggleCameraView();
	void ChangeCharacter();
		
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
	void CalculateHealthPercent();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCollectedKeyUI();
};
