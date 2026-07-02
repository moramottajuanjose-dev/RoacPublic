#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RHProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

/**
 * ARHProjectileBase
 *
 * Base class for all fired projectiles (machine gun rounds, missiles, etc).
 * Moves in a straight line via UProjectileMovementComponent, and applies
 * damage to whatever it hits that has a URHHealthComponent.
 *
 * Subclasses (e.g. ARHMachineGun) override the defaults in their
 * constructor to get different speed/damage/lifespan characteristics
 * without duplicating the hit/damage logic.
 */
UCLASS()
class ROADHAVOC_API ARHProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ARHProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	/** Damage applied to the actor hit, if it has a URHHealthComponent. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Damage = 25.f;

	/** Muzzle velocity in cm/s. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float InitialSpeed = 6000.f;

	/** Seconds before the projectile self-destructs if it hits nothing. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LifeSpanSeconds = 3.f;

	/** If true, the projectile also damages the actor that fired it (usually false). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bCanDamageInstigator = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	/** Hook for subclasses/Blueprints to spawn impact VFX/SFX before the projectile is destroyed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
	void OnImpactEffects(const FHitResult& Hit);
};
