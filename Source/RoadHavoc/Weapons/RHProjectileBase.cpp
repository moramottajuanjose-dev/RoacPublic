#include "Weapons/RHProjectileBase.h"
#include "Vehicle/RHHealthComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARHProjectileBase::ARHProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ARHProjectileBase::OnProjectileHit);
	RootComponent = CollisionComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = InitialSpeed;
	MovementComponent->MaxSpeed = InitialSpeed;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->bRotationFollowsVelocity = true;
}

void ARHProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Keep the movement component in sync with any per-instance/subclass override
	// of InitialSpeed made in a Blueprint or a subclass constructor.
	MovementComponent->InitialSpeed = InitialSpeed;
	MovementComponent->MaxSpeed = InitialSpeed;
	MovementComponent->Velocity = GetActorForwardVector() * InitialSpeed;

	SetLifeSpan(LifeSpanSeconds);
}

void ARHProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}

	if (!bCanDamageInstigator && OtherActor == GetInstigator())
	{
		return;
	}

	if (URHHealthComponent* HealthComponent = OtherActor->FindComponentByClass<URHHealthComponent>())
	{
		HealthComponent->ApplyDamage(Damage);
	}

	OnImpactEffects(Hit);

	Destroy();
}
