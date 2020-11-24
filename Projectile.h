// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	PointDamage = 0 UMETA(DisplayName = "PointDamage"),
	RadialDamage = 1 UMETA(DisplayName = "RadialDamage"),
};

UCLASS()
class HIRS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		class UParticleSystem* ProjectileParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		class UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		class USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		class UParticleSystem* ProjectileHitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
		class UParticleSystem* ProjectileSmokeTrailParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* SphereCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, Category = "Damage")
		float DamageRadius;

	UPROPERTY(EditAnywhere, Category = "Damage")
		TEnumAsByte<EDamageType> DamageTypeEnum;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnParticles();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
