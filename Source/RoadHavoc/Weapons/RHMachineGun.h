#pragma once

#include "CoreMinimal.h"
#include "Weapons/RHProjectileBase.h"
#include "RHMachineGun.generated.h"

/**
 * ARHMachineGun
 *
 * Machine gun bullet: fast, low damage, short lifespan, no gravity.
 * This is the projectile spawned by URHWeaponMountComponent::FirePrimary()
 * on the default vehicle loadout.
 */
UCLASS()
class ROADHAVOC_API ARHMachineGun : public ARHProjectileBase
{
	GENERATED_BODY()

public:
	ARHMachineGun();
};
