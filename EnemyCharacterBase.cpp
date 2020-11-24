// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponObject.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "HealthComponent.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AIPerceptionComp*/
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsDied = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AEnemyCharacterBase::OnHealthChanged);
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::Fire()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->CurrentMagazineAmmo > 0 && !CurrentWeapon->IsReloading)
		{
			if (GetMesh()->GetAnimInstance()->Montage_GetIsStopped(CurrentWeapon->FireMontage))
			{
				if (CurrentWeapon->FireMontage)
				{
					GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->FireMontage);
				}
				CurrentWeapon->FireShot(this);
			}
		}
		else
		{
			CurrentWeapon->Reload();
		}
	}
}

void AEnemyCharacterBase::OnHealthChanged(UHealthComponent* HealthComponent, float Health)
{
	if (Health <= 0.f && !bIsDied)
	{
		GetCharacterMovement()->StopMovementImmediately();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bIsDied = true;

	}
	/*if (bIsDied)
	{
		
	}*/
}

void AEnemyCharacterBase::Die()
{
	DetachFromControllerPendingDestroy();

	SetLifeSpan(10.f);
}
