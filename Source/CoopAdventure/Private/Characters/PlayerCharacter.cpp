
#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.15f;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f; 
	SpringArm->bUsePawnControlRotation = true;
	StartArmLength = SpringArm->TargetArmLength;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bIsFirstPerson = false;
	bInvertX = false;
	bInvertY = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CalculateHealthPercent();
	DisableCrosshair();
	
	GetWorldTimerManager().SetTimer(checkForPossessableCharacterTimer, this, &APlayerCharacter::CheckForPossessableCharacter, .5f, true);
	
	CollectableKey = Cast<ACollectableKey>(UGameplayStatics::GetActorOfClass(GetWorld(), ACollectableKey::StaticClass()));
	if (CollectableKey)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Collectable Key");
		CollectableKey->OnUpdateUI.AddDynamic(this, &APlayerCharacter::UpdateCollectedKeyUI);
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InterpCameraIfRunning(DeltaTime);

	if (IsLocallyControlled())
	{
		ToggleCrosshair();
	}
}

void APlayerCharacter::CheckForPossessableCharacter()
{
	FVector startLocation = FollowCamera->GetComponentLocation();
	FVector endLocation = startLocation + FollowCamera->GetForwardVector() * 3000.f;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult hitResult;
	bool hasHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_Camera, CollisionParams);

	if (IsLocallyControlled())
	{
		if (hasHit && hitResult.GetActor()->ActorHasTag("Player"))
		{
			ShowPossesMessage();
		}else
		{
			HidePossesMessage();
		}
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ToggleCrosshair();
}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();
	DisableCrosshair();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(ToggleCameraViewAction, ETriggerEvent::Completed, this, &APlayerCharacter::ToggleCameraView);
		EnhancedInputComponent->BindAction(ChangeCharacterAction, ETriggerEvent::Completed, this, &APlayerCharacter::ChangeCharacter);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &APlayerCharacter::Pause);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		if (bInvertX)
		{
			AddControllerYawInput(-LookAxisVector.X);
		}
		else
		{
			AddControllerYawInput(LookAxisVector.X);
		}

		if (bInvertY)
		{
			AddControllerPitchInput(-LookAxisVector.Y);
		}else
		{
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void APlayerCharacter::StartSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bIsSprinting = true;
	
}

void APlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	bIsSprinting = false;
}

void APlayerCharacter::Pause()
{
	if (!bIsGamePaused)
	{
		bIsGamePaused = true;
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeGameAndUI());
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
		PauseGame();
	}else
	{
		bIsGamePaused = false;
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(false);
			PlayerController->SetInputMode(FInputModeGameOnly());
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		UnPauseGame();
	}
}

void APlayerCharacter::ToggleCameraView()
{
	if (!bIsFirstPerson)
	{
		SpringArm->TargetArmLength = 0.f;
		SpringArm->SetRelativeLocation(FVector(10.0f, 0.0f, 70.0f)); 
		FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
		GetMesh()->SetOwnerNoSee(true);
		bIsFirstPerson = true;
	}
	else
	{
		SpringArm->TargetArmLength = StartArmLength;
		SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); 
		FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
		GetMesh()->SetOwnerNoSee(false);
		bIsFirstPerson = false;
	}
}


void APlayerCharacter::ChangeCharacter()
{
	FVector startLocation = FollowCamera->GetComponentLocation();
	FVector endLocation = startLocation + FollowCamera->GetForwardVector() * 3000.f;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult hitResult;
	bool hasHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_Camera, CollisionParams);

	if (hasHit && hitResult.GetActor()->ActorHasTag("Player"))
	{
		PossesOtherCharacter(hitResult);
	}
}

void APlayerCharacter::PossesOtherCharacter(FHitResult hitResult)
{
	APlayerCharacter* HitCharacter = Cast<APlayerCharacter>(hitResult.GetActor());
	if (HitCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->Possess(HitCharacter);
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();
			StopSprinting();
			HidePossesMessage();
		}
	}
}

void APlayerCharacter::ToggleCrosshair()
{
	if (bIsFirstPerson)
	{
		EnableCrosshair();
	}
	else
	{
		DisableCrosshair();
	}
}

void APlayerCharacter::InterpCameraIfRunning(float DeltaTime) const
{
	if (SpringArm)
	{
		if (bIsSprinting)
		{
			if (bIsFirstPerson) return;
			SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, ArmLengthInterpSpeed);
		}else
		{
			if (bIsFirstPerson) return;
			SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, StartArmLength, DeltaTime, ArmLengthInterpSpeed);
		}
	}
}

