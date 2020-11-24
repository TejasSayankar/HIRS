// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponObject.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "FPSPlayer.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"



UWeaponObject::UWeaponObject()
{

	MuzzleSocketName1 = "Enter the Muzzle Socket Name1";
	MuzzleSocketName2 = "Enter the Muzzle Socket Name2";

	IsReloading = false;
	WeaponName = "Enter the Weapon Name";
}

void UWeaponObject::FireShot(ACharacter* MyPawn)
{

	FVector EyeLocation;
	FRotator EyeRotation;


	MyPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	/*FVector ShotDirection = EyeRotation.Vector();

	float BulletSpreadRad = FMath::DegreesToRadians(BulletSpread);
	EyeRotation = FMath::RadiansToDegrees(FMath::VRandCone(ShotDirection, BulletSpread, BulletSpread));*/

	EyeRotation = EyeRotation + (FRotator(RandRotation()));

	if (MuzzleParticles)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, MyPawn->GetMesh(), MuzzleSocketName1);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName1));
	}

	//Throw(MyPawn, EyeRotation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = MyPawn;
	Fired_Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName1), EyeRotation, SpawnParams);

	
	CurrentMagazineAmmo--;
}

void UWeaponObject::ReloadWithTimer()
{
	IsReloading = true;
	//GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &AFPSPlayer::Reload, CurrentWeapon->, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadTime, this, &UWeaponObject::Reload, ReloadWaitTime, true);
}

float UWeaponObject::RandRotation()
{
	return FMath::FRandRange(-BulletSpread, BulletSpread);
}

void UWeaponObject::Refill()
{
	this->CurrentAmmo = this->MaxAmmo;
}

void UWeaponObject::FireShot2(ACharacter* MyPawn)
{
	FVector EyeLocation;
	FRotator EyeRotation;


	MyPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	EyeRotation = EyeRotation + (FRotator(RandRotation()));

	if (MuzzleParticles)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticles, MyPawn->GetMesh(), MuzzleSocketName2);
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName2));
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = MyPawn;
	Fired_Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName2), EyeRotation, SpawnParams);
}

//void UWeaponObject::Throw(ACharacter* MyPawn)
//{
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//	SpawnParams.Owner = MyPawn;
//	Fired_Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName), EyeRotation, SpawnParams);
//}

void UWeaponObject::Reload()
{
	if (CurrentAmmo >= MagazineCapacity - CurrentMagazineAmmo)
	{
		CurrentAmmo -= (MagazineCapacity - CurrentMagazineAmmo);
		CurrentMagazineAmmo = MagazineCapacity;
	}
	else
	{
		CurrentMagazineAmmo += CurrentAmmo;
		CurrentAmmo = 0;
	}
	
	IsReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadTime);
}
