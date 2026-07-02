#include "Vehicle/RHCameraComponent.h"

URHCameraComponent::URHCameraComponent()
{
    PrimaryComponentTick.bCanEverTick=false;
}

void URHCameraComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URHCameraComponent::ToggleLookBack(bool bEnable)
{
    bLookingBack=bEnable;
}
