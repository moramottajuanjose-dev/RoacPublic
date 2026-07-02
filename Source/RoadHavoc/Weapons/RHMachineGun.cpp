#include "Weapons/RHMachineGun.h"

ARHMachineGun::ARHMachineGun()
{
	Damage = 12.f;
	InitialSpeed = 9000.f;
	LifeSpanSeconds = 1.5f;

	if (CollisionComponent)
	{
		CollisionComponent->InitSphereRadius(4.f);
	}
}
