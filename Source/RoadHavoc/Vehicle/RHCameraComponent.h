#pragma once
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RHCameraComponent.generated.h"

UCLASS(ClassGroup=(RoadHavoc), meta=(BlueprintSpawnableComponent))
class ROADHAVOC_API URHCameraComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    URHCameraComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefaultArmLength = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LookBackYaw = 180.f;

    UFUNCTION(BlueprintCallable)
    void ToggleLookBack(bool bEnable);

protected:
    virtual void BeginPlay() override;

private:
    bool bLookingBack=false;
};
