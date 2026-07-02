#include "Vehicle/RHWeaponMountComponent.h"
#include "Weapons/RHProjectileBase.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"

URHWeaponMountComponent::URHWeaponMountComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool URHWeaponMountComponent::CanFirePrimary() const
{
	if (!GetWorld())
	{
		return false;
	}
	return (GetWorld()->GetTimeSeconds() - LastPrimaryFireTime) >= FireRate;
}

bool URHWeaponMountComponent::CanFireSecondary() const
{
	if (!GetWorld())
	{
		return false;
	}
	return (GetWorld()->GetTimeSeconds() - LastSecondaryFireTime) >= SecondaryFireRate;
}

bool URHWeaponMountComponent::FirePrimary()
{
	if (!ProjectileClass || !CanFirePrimary())
	{
		return false;
	}

	if (SpawnProjectile(ProjectileClass))
	{
		LastPrimaryFireTime = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

bool URHWeaponMountComponent::FireSecondary()
{
	const TSubclassOf<ARHProjectileBase> ClassToSpawn = SecondaryProjectileClass ? SecondaryProjectileClass : ProjectileClass;

	if (!ClassToSpawn || !CanFireSecondary())
	{
		return false;
	}

	if (SpawnProjectile(ClassToSpawn))
	{
		LastSecondaryFireTime = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

ARHProjectileBase* URHWeaponMountComponent::SpawnProjectile(TSubclassOf<ARHProjectileBase> SpawnClass)
{
	UWorld* World = GetWorld();
	if (!World || !SpawnClass)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		SpawnParams.Instigator = OwnerPawn;
	}
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform SpawnTransform = GetComponentTransform();

	return World->SpawnActor<ARHProjectileBase>(SpawnClass, SpawnTransform, SpawnParams);
}
