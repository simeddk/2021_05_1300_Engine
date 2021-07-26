#include "CThrow.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ThrowEffect, "ThrowEffect", Sphere);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 3.0f;
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	if (!!HitEffect)
	{
		FTransform transform = HitEffectTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

