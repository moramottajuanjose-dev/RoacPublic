#include "Vehicle/RHVehicleBase.h"
#include "Vehicle/RHHealthComponent.h"
#include "Vehicle/RHNitroComponent.h"
#include "Vehicle/RHCameraComponent.h"
#include "Vehicle/RHWeaponMountComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

ARHVehicleBase::ARHVehicleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// --- Camera rig -------------------------------------------------
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// --- Gameplay components -----------------------------------------
	HealthComponent = CreateDefaultSubobject<URHHealthComponent>(TEXT("HealthComponent"));
	NitroComponent = CreateDefaultSubobject<URHNitroComponent>(TEXT("NitroComponent"));
	CameraLogicComponent = CreateDefaultSubobject<URHCameraComponent>(TEXT("CameraLogicComponent"));

	FrontWeaponMount = CreateDefaultSubobject<URHWeaponMountComponent>(TEXT("FrontWeaponMount"));
	FrontWeaponMount->Slot = ERHWeaponSlot::Front;

	RearWeaponMount = CreateDefaultSubobject<URHWeaponMountComponent>(TEXT("RearWeaponMount"));
	RearWeaponMount->Slot = ERHWeaponSlot::Rear;
}

void ARHVehicleBase::BeginPlay()
{
	Super::BeginPlay();

	if (SpringArm)
	{
		SpringArm->TargetArmLength = CameraLogicComponent
			? CameraLogicComponent->DefaultArmLength
			: SpringArm->TargetArmLength;
	}

	if (HealthComponent)
	{
		HealthComponent->OnVehicleDestroyed.AddDynamic(this, &ARHVehicleBase::HandleVehicleDestroyed);
	}

	// Add the Enhanced Input mapping context for the local player controlling this pawn.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
					LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (VehicleMappingContext)
				{
					Subsystem->AddMappingContext(VehicleMappingContext, 0);
				}
			}
		}
	}
}

void ARHVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AccelerateAction)
		{
			EIC->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &ARHVehicleBase::Input_Accelerate);
			EIC->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ARHVehicleBase::Input_Accelerate);
		}
		if (BrakeAction)
		{
			EIC->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ARHVehicleBase::Input_Brake);
			EIC->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ARHVehicleBase::Input_Brake);
		}
		if (SteerAction)
		{
			EIC->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ARHVehicleBase::Input_Steer);
			EIC->BindAction(SteerAction, ETriggerEvent::Completed, this, &ARHVehicleBase::Input_Steer);
		}
		if (FireAction)
		{
			EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ARHVehicleBase::Input_Fire);
		}
		if (NitroAction)
		{
			EIC->BindAction(NitroAction, ETriggerEvent::Started, this, &ARHVehicleBase::Input_NitroStart);
			EIC->BindAction(NitroAction, ETriggerEvent::Completed, this, &ARHVehicleBase::Input_NitroStop);
			EIC->BindAction(NitroAction, ETriggerEvent::Canceled, this, &ARHVehicleBase::Input_NitroStop);
		}
		if (LookBackAction)
		{
			EIC->BindAction(LookBackAction, ETriggerEvent::Started, this, &ARHVehicleBase::Input_LookBackStart);
			EIC->BindAction(LookBackAction, ETriggerEvent::Completed, this, &ARHVehicleBase::Input_LookBackStop);
			EIC->BindAction(LookBackAction, ETriggerEvent::Canceled, this, &ARHVehicleBase::Input_LookBackStop);
		}
	}
}

void ARHVehicleBase::Input_Accelerate(const FInputActionValue& Value)
{
	SetThrottleInput(Value.Get<float>());
}

void ARHVehicleBase::Input_Brake(const FInputActionValue& Value)
{
	SetBrakeInput(Value.Get<float>());
}

void ARHVehicleBase::Input_Steer(const FInputActionValue& Value)
{
	SetSteeringInput(Value.Get<float>());
}

void ARHVehicleBase::Input_Fire(const FInputActionValue& Value)
{
	if (FrontWeaponMount)
	{
		FrontWeaponMount->FirePrimary();
	}
}

void ARHVehicleBase::Input_NitroStart(const FInputActionValue& Value)
{
	if (NitroComponent)
	{
		NitroComponent->StartBoost();
	}
}

void ARHVehicleBase::Input_NitroStop(const FInputActionValue& Value)
{
	if (NitroComponent)
	{
		NitroComponent->StopBoost();
	}
}

void ARHVehicleBase::Input_LookBackStart(const FInputActionValue& Value)
{
	if (CameraLogicComponent)
	{
		CameraLogicComponent->ToggleLookBack(true);
	}
	if (SpringArm && CameraLogicComponent)
	{
		SpringArm->SetRelativeRotation(FRotator(0.f, CameraLogicComponent->LookBackYaw, 0.f));
	}
}

void ARHVehicleBase::Input_LookBackStop(const FInputActionValue& Value)
{
	if (CameraLogicComponent)
	{
		CameraLogicComponent->ToggleLookBack(false);
	}
	if (SpringArm)
	{
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void ARHVehicleBase::HandleVehicleDestroyed()
{
	// TODO: trigger destruction VFX/SFX, disable input, notify RHMatchManager.
	SetActorEnableCollision(false);
}
