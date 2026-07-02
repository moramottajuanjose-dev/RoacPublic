#include "Vehicle/RHNitroComponent.h"
#include "Math/UnrealMathUtility.h"

URHNitroComponent::URHNitroComponent(){
 PrimaryComponentTick.bCanEverTick=true;
}

void URHNitroComponent::TickComponent(float DeltaTime,ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction){
 Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
 if(bBoosting)
   CurrentNitro=FMath::Max(0.f,CurrentNitro-ConsumePerSecond*DeltaTime);
 else
   CurrentNitro=FMath::Min(MaxNitro,CurrentNitro+RegenPerSecond*DeltaTime);
 OnNitroChanged.Broadcast(CurrentNitro,MaxNitro);
 if(CurrentNitro<=0.f) bBoosting=false;
}
void URHNitroComponent::StartBoost(){ if(CurrentNitro>0) bBoosting=true; }
void URHNitroComponent::StopBoost(){ bBoosting=false; }
