#pragma once
#include "Components/ActorComponent.h"
#include "RHWeaponMountComponent.generated.h"

UENUM(BlueprintType)
enum class ERHWeaponSlot:uint8
{
    Front,
    Rear,
    Left,
    Right
};

UCLASS(ClassGroup=(RoadHavoc), meta=(BlueprintSpawnableComponent))
class ROADHAVOC_API URHWeaponMountComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    URHWeaponMountComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERHWeaponSlot Slot = ERHWeaponSlot::Front;

    UFUNCTION(BlueprintCallable)
    void FirePrimary();

    UFUNCTION(BlueprintCallable)
    void FireSecondary();
};
