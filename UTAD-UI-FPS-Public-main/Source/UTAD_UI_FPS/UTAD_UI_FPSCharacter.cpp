// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTAD_UI_FPSCharacter.h"
#include "UTAD_UI_FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// UI
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Skills/SkillsComponent.h"
#include "UI/GameOver.h"
#include "UI/PlayerHUD.h"

//////////////////////////////////////////////////////////////////////////
// AUTAD_UI_FPSCharacter

AUTAD_UI_FPSCharacter::AUTAD_UI_FPSCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	SkillsComponent = CreateDefaultSubobject<USkillsComponent>("SkillsCmp");
}

void AUTAD_UI_FPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->SetShowMouseCursor(false);
	}

	// Setup Player HUD
	if (PlayerHUDWidget)
	{
		PlayerHUDInstance = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidget);
		PlayerHUDInstance->AddToViewport();
		PlayerHUDInstance->ShowNoWeapon();
		PlayerHUDInstance->HideSkillTree();

		SetHealth(MaxHealth, FVector::Zero());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
	}

	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
	InitialJumpForce = GetCharacterMovement()->JumpZVelocity;
	CurrentDamage = InitialDamage;
}

//////////////////////////////////////////////////////////////////////////// Input

void AUTAD_UI_FPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::Look);

		EnhancedInputComponent->BindAction(CloseSplashAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::CloseSplashScreenInput);

		EnhancedInputComponent->BindAction(SkillTreeAction, ETriggerEvent::Triggered, this, &AUTAD_UI_FPSCharacter::ShowInventory);
	}
}

void AUTAD_UI_FPSCharacter::CloseSplashScreenInput()
{
	if(!UsingUiInput) return;
	
	if(OnSkipScreen.IsBound())
	{
		OnSkipScreen.Execute();
	}

	CloseSplashScreen();
}

void AUTAD_UI_FPSCharacter::CloseSplashScreen()
{
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->SetShowMouseCursor(false);
	}
}

void AUTAD_UI_FPSCharacter::SetSpeed(const float Multiplier)
{
	GetCharacterMovement()->MaxWalkSpeed = InitialSpeed * Multiplier;
	UE_LOG(LogTemp, Warning, TEXT("New Speed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void AUTAD_UI_FPSCharacter::SetJumpForce(const float Multiplier)
{
	GetCharacterMovement()->JumpZVelocity = InitialJumpForce * Multiplier;
	UE_LOG(LogTemp, Warning, TEXT("New Jump Force: %f"), GetCharacterMovement()->JumpZVelocity);
}

void AUTAD_UI_FPSCharacter::SetDamage(const float Multiplier)
{
	CurrentDamage = InitialDamage * Multiplier;
	UE_LOG(LogTemp, Warning, TEXT("New Damage: %f"), CurrentDamage);
}

void AUTAD_UI_FPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), CurrentDamage);
}

void AUTAD_UI_FPSCharacter::ShowInventory()
{
	if(UsingUiInput) return;
	
	PlayerHUDInstance->ShowSkillTree();
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(true);
		DisableInput(PlayerController);
		FInputModeUIOnly InputModeUIOnly;
		PlayerController->SetInputMode(InputModeUIOnly);
	}
}

void AUTAD_UI_FPSCharacter::HideInventory()
{
	PlayerHUDInstance->HideSkillTree();
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(false);
		EnableInput(PlayerController);
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
	}
}

void AUTAD_UI_FPSCharacter::Move(const FInputActionValue& Value)
{
	if(UsingUiInput) return;
	
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUTAD_UI_FPSCharacter::StartJump()
{
	if(UsingUiInput) return;
	
	Jump();
}

void AUTAD_UI_FPSCharacter::Look(const FInputActionValue& Value)
{
	if(UsingUiInput) return;
	
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUTAD_UI_FPSCharacter::SetHealth(int NewHealth, const FVector& ImpactLocation)
{
	int ClampedNewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	if (ClampedNewHealth != Health)
	{
		Health = ClampedNewHealth;
		if(OnHealthChanged.IsBound())
		{
			OnHealthChanged.Execute(Health, MaxHealth);
		}
		if(ImpactLocation != FVector::Zero() && OnReceiveDamage.IsBound())
		{
			OnReceiveDamage.Execute(this, ImpactLocation);
		}
		// Setup GameOver Screen
		if(Health == 0)
		{
			if (GameOverWidget)
			{
				GameOverInstance = CreateWidget<UGameOver>(GetWorld(), GameOverWidget);
				GameOverInstance->AddToViewport();
				GameOverInstance->Show();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
			}
		}
	}
}

int AUTAD_UI_FPSCharacter::GetHealth()
{
	return Health;
}

void AUTAD_UI_FPSCharacter::SetMaxHealth(int NewMaxHealth)
{
	MaxHealth = FMath::Max(0, NewMaxHealth);
}

int AUTAD_UI_FPSCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void AUTAD_UI_FPSCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
	if(bNewHasRifle)
	{
		PlayerHUDInstance->ShowWeapon();
		if(OnTotalBulletsChanged.IsBound())
		{
			OnTotalBulletsChanged.Execute(TotalBullets);
		}
	}
}

bool AUTAD_UI_FPSCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AUTAD_UI_FPSCharacter::SetTotalBullets(int NewTotalBullets)
{
	TotalBullets = NewTotalBullets;
	if(OnTotalBulletsChanged.IsBound())
	{
		OnTotalBulletsChanged.Execute(NewTotalBullets);
	}
}

int AUTAD_UI_FPSCharacter::GetTotalBullets()
{
	return TotalBullets;
}

void AUTAD_UI_FPSCharacter::AddBullets(int Bullets)
{
	TotalBullets += Bullets;
}

void AUTAD_UI_FPSCharacter::SetAttachedWeaponComponent(UTP_WeaponComponent* WeaponComponent)
{
	AttachedWeaponComponent = WeaponComponent;
}
