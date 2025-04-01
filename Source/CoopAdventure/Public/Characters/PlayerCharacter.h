
#pragma once

#include "CoreMinimal.h"
#include "CollectableKey.h"
#include "GameFramework/Character.h"
#include "Items/Item.h"
#include "SaveSystem/BodySwapGameInstance.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class COOPADVENTURE_API APlayerCharacter : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetOverlappingItem(class AItem* item) override;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprinting();
	void StopSprinting();
	void Pause();
	void EKeyPressed();


	
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickupAction;
	
	// objects in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACollectableKey* CollectableKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AItem* OverlappingItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	AItem* ItemInHand;
	
	//sprinting
	bool bIsSprinting;
	bool bIsFirstPerson;

	
	float StartArmLength;
	float TargetArmLength = 500.f;
	float ArmLengthInterpSpeed = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float sensitivity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bInvertX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bInvertY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsGamePaused;

	FTimerHandle checkForPossessableCharacterTimer;
	void CheckForPossessableCharacter();
	
	void InterpCameraIfRunning(float DeltaTime) const;
	void ToggleCameraView();
	void PossesOtherCharacter(FHitResult hitResult);
	void ChangeCharacter();
	void ToggleCrosshair();

	UPROPERTY()
	UBodySwapGameInstance* bodySwapGameInstance;
		
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//blueprint implementable events
	UFUNCTION(BlueprintImplementableEvent)
	void CalculateHealthPercent();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCollectedKeyUI();
	UFUNCTION(BlueprintImplementableEvent)
	void EnableCrosshair();
	UFUNCTION(BlueprintImplementableEvent)
	void DisableCrosshair();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowPossesMessage();
	UFUNCTION(BlueprintImplementableEvent)
	void HidePossesMessage();
	UFUNCTION(BlueprintImplementableEvent)
	void PauseGame();
	UFUNCTION(BlueprintImplementableEvent)
	void UnPauseGame();

};



