#include "Vehicle/RHWeaponMountComponent.h"

URHWeaponMountComponent::URHWeaponMountComponent()
{
    PrimaryComponentTick.bCanEverTick=false;
}

void URHWeaponMountComponent::FirePrimary()
{
    // TODO: Spawn projectile
}

void URHWeaponMountComponent::FireSecondary()
{
    // TODO: Launch special weapon
}
