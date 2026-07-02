#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RHWeaponMountComponent.generated.h"

class ARHProjectileBase;

UENUM(BlueprintType)
enum class ERHWeaponSlot : uint8
{
	Front,
	Rear,
	Left,
	Right
};

/**
 * URHWeaponMountComponent
 *
 * Represents a single weapon hardpoint on a vehicle (front bumper, roof
 * turret, etc). It is a USceneComponent (not a plain UActorComponent) so it
 * can be attached to the vehicle mesh at a socket/offset and provide a
 * world-space spawn transform for projectiles.
 *
 * FirePrimary()/FireSecondary() spawn ProjectileClass/SecondaryProjectileClass
 * at this component's world transform, respecting FireRate as a per-slot
 * cooldown so mashing the fire button doesn't bypass rate-of-fire.
 */
UCLASS(ClassGroup=(RoadHavoc), meta=(BlueprintSpawnableComponent))
class ROADHAVOC_API URHWeaponMountComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URHWeaponMountComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ERHWeaponSlot Slot = ERHWeaponSlot::Front;

	/** Projectile class spawned by FirePrimary(). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARHProjectileBase> ProjectileClass;

	/** Projectile class spawned by FireSecondary(). Defaults to ProjectileClass if unset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARHProjectileBase> SecondaryProjectileClass;

	/** Minimum seconds between primary shots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.15f;

	/** Minimum seconds between secondary shots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float SecondaryFireRate = 1.5f;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FirePrimary();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FireSecondary();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanFirePrimary() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	bool CanFireSecondary() const;

protected:
	/** Spawns SpawnClass at this component's current world transform, owned/instigated by the mount's owner. */
	ARHProjectileBase* SpawnProjectile(TSubclassOf<ARHProjectileBase> SpawnClass);

private:
	// Large negative sentinel meaning "never fired yet", so the first shot is never blocked by cooldown.
	double LastPrimaryFireTime = -1000000.0;
	double LastSecondaryFireTime = -1000000.0;
};
