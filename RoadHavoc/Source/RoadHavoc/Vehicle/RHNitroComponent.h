#pragma once
#include "Components/ActorComponent.h"
#include "RHNitroComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNitroChanged,float,Current,float,Max);

UCLASS(ClassGroup=(RoadHavoc), meta=(BlueprintSpawnableComponent))
class ROADHAVOC_API URHNitroComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    URHNitroComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxNitro = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float CurrentNitro = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RegenPerSecond = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConsumePerSecond = 25.f;

    UPROPERTY(BlueprintAssignable)
    FOnNitroChanged OnNitroChanged;

    virtual void TickComponent(float DeltaTime,ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void StartBoost();

    UFUNCTION(BlueprintCallable)
    void StopBoost();

private:
    bool bBoosting=false;
};
