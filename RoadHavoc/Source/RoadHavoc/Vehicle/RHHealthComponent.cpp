#include "Vehicle/RHHealthComponent.h"

URHHealthComponent::URHHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentHealth = MaxHealth;
}

void URHHealthComponent::ApplyDamage(float Damage)
{
    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
    if(CurrentHealth <= 0.f)
        OnVehicleDestroyed.Broadcast();
}

void URHHealthComponent::Heal(float Amount)
{
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

bool URHHealthComponent::IsDestroyed() const
{
    return CurrentHealth <= 0.f;
}
