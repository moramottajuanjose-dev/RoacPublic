#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehiclePawn.h"
#include "RHVehicleBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URHHealthComponent;
class URHNitroComponent;
class URHCameraComponent;
class URHWeaponMountComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * ARHVehicleBase
 *
 * Base vehicle pawn for all Road Havoc vehicles.
 *
 * Architecture note: earlier prototypes (Commit 0006/0007) put Health/Nitro
 * fields directly on this class. That approach was superseded by the
 * component-based design built in Commits 0008-0011
 * (URHHealthComponent / URHNitroComponent / URHCameraComponent /
 * URHWeaponMountComponent). This class is the integration point those
 * commits were building toward: it owns one instance of each component and
 * wires Enhanced Input to them, plus to Chaos Vehicles' native
 * throttle/steering/brake inputs.
 */
UCLASS()
class ROADHAVOC_API ARHVehicleBase : public AChaosWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ARHVehicleBase();

	// --- Components -----------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Health")
	TObjectPtr<URHHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Nitro")
	TObjectPtr<URHNitroComponent> NitroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Camera")
	TObjectPtr<URHCameraComponent> CameraLogicComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Weapons")
	TObjectPtr<URHWeaponMountComponent> FrontWeaponMount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle|Weapons")
	TObjectPtr<URHWeaponMountComponent> RearWeaponMount;

	// --- Enhanced Input assets (assign in Blueprint/editor) -------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputMappingContext> VehicleMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> AccelerateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> BrakeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> SteerAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> NitroAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vehicle|Input")
	TObjectPtr<UInputAction> LookBackAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Enhanced Input callbacks
	void Input_Accelerate(const FInputActionValue& Value);
	void Input_Brake(const FInputActionValue& Value);
	void Input_Steer(const FInputActionValue& Value);
	void Input_Fire(const FInputActionValue& Value);
	void Input_NitroStart(const FInputActionValue& Value);
	void Input_NitroStop(const FInputActionValue& Value);
	void Input_LookBackStart(const FInputActionValue& Value);
	void Input_LookBackStop(const FInputActionValue& Value);

	// Component event handlers
	UFUNCTION()
	void HandleVehicleDestroyed();
};
