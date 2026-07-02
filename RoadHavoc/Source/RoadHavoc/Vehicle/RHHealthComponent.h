#pragma once
#include "Components/ActorComponent.h"
#include "RHHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged,float,CurrentHealth,float,MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVehicleDestroyed);

UCLASS(ClassGroup=(RoadHavoc), meta=(BlueprintSpawnableComponent))
class ROADHAVOC_API URHHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URHHealthComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float MaxHealth = 1000.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
    float CurrentHealth;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

    UPROPERTY(BlueprintAssignable)
    FOnVehicleDestroyed OnVehicleDestroyed;

    UFUNCTION(BlueprintCallable)
    void ApplyDamage(float Damage);

    UFUNCTION(BlueprintCallable)
    void Heal(float Amount);

    UFUNCTION(BlueprintPure)
    bool IsDestroyed() const;
};
