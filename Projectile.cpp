

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollisionComponent;
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->UpdatedComponent = SphereCollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 10000.f;
	ProjectileMovementComponent->MaxSpeed = 10000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	InitialLifeSpan = 5.0;

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SpawnParticles();
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::SpawnParticles()
{
	if (ProjectileParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(ProjectileParticle, SphereCollisionComponent);
	}
	if (ProjectileSmokeTrailParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(ProjectileSmokeTrailParticle, SphereCollisionComponent);
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetOwner())
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		//IgnoredActors.Add(GetOwner());
		switch (DamageTypeEnum)
		{
		case EDamageType::PointDamage:
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorRotation().Vector(), Hit, GetOwner()->GetInstigatorController(), GetOwner(), DamageType);
			//UGameplayStatics::ApplyPointDamage()
			break;

		case EDamageType::RadialDamage:
			if (UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageType, IgnoredActors, GetOwner(), GetOwner()->GetInstigatorController(), true))
			{
				//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 15.f, FColor::Yellow, false, 3.f);
			}
			break;
		default:
			break;
		}


		if (ProjectileHitParticle) 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileHitParticle, Hit.ImpactPoint);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.ImpactPoint);
		}
		if (ExplosionParticle) 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, Hit.ImpactPoint);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Hit"));
		Destroy();
	}
	
	
	
}


