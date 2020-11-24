// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponObject.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class HIRS_API UWeaponObject : public UObject
{
	GENERATED_BODY()
	
public:
	UWeaponObject();

protected:

	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		class USoundBase* FireSound;

	class AProjectile* Fired_Projectile;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Socket")
		FName MuzzleSocketName1;

	UPROPERTY(EditAnywhere, Category = "Socket")
		FName MuzzleSocketName2;

	UPROPERTY(EditAnywhere, Category = "Fire")
		float BulletSpread;

public:

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		int MagazineCapacity;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		int CurrentMagazineAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		int MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		int CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Range;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		int FireRate;

	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float ReloadWaitTime;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		class UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		class UAnimMontage* ReloadMontage;

	void FireShot(ACharacter* MyPawn);

	void Reload();

	void ReloadWithTimer();

	float RandRotation();

	bool IsReloading;

	FTimerHandle TimerHandle_ReloadTime;

	void Refill();

	UFUNCTION(BlueprintCallable)
	void FireShot2(ACharacter* MyPawn);

	/*UFUNCTION(BlueprintCallable)
		void Throw(ACharacter* MyPawn);*/
};
