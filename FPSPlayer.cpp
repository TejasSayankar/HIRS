// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components\CapsuleComponent.h"
#include "Interactable.h"
#include "WeaponObject.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "Medkit.h"


AFPSPlayer::AFPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleSize(30.f, 88.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetMesh(),"head");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 0.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera"); 
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	CurrentInteractable = nullptr;

	HelpText = "";

	CanFire = true;

	IsReloading = false;
	StimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComp"));
	SetupStimulus();
	
	bIsDied = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AFPSPlayer::OnHealthChanged);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->Items.SetNum(10);

	ObjectiveText = "Investigate the lab";
	ObjectiveCount = 0;

	CameraSwitch = false;
	bIsGameOver = false;
}

void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	EquipRevolver();
	
}

void AFPSPlayer::SetupStimulus()
{
	
	StimuliSourceComp->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSourceComp->RegisterWithPerceptionSystem();
}

void AFPSPlayer::MoveForward(float Value)
{
	if (Controller && Value != 0.f) 
	{
		FRotator ControlRotation = GetControlRotation();
		FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	
}

void AFPSPlayer::MoveRight(float Value)
{
	if (Controller && Value != 0)
	{
		FRotator ControlRotation = GetControlRotation();
		FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AFPSPlayer::Jumping()
{
	Jump();
}

void AFPSPlayer::StopJump()
{
	StopJumping();
}


void AFPSPlayer::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact_Implementation(this);
	}

}

void AFPSPlayer::CheckForInteractables()
{
	FHitResult HitResult;
	FVector StartTrace = Camera->GetComponentLocation();
	FVector EndTrace = (Camera->GetForwardVector() * 250.f) + StartTrace;
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	AInteractable* Interactable;

	Interactable = Cast<AInteractable>(HitResult.GetActor());

	if (Interactable)
	{
		CurrentInteractable = Interactable;
		HelpText = Interactable->InteractableHelpText;
	}
	else
	{
		CurrentInteractable = nullptr;
		HelpText = "";
	}
	
}

void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();

	IsReloading = CurrentWeapon->IsReloading;
	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, TEXT("Hit"));

}

void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSPlayer::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSPlayer::StopJump);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSPlayer::Interact);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::Fire);

	PlayerInputComponent->BindAction("Revolver", IE_Pressed, this, &AFPSPlayer::EquipRevolver);
	PlayerInputComponent->BindAction("Launcher", IE_Pressed, this, &AFPSPlayer::EquipLauncher);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AFPSPlayer::EquipGrenade);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSPlayer::Reload);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AFPSPlayer::SwitchWeapon);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AFPSPlayer::Heal);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AFPSPlayer::NextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AFPSPlayer::PreviousWeapon);

	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AFPSPlayer::SwitchCamera);

}

FVector AFPSPlayer::GetPawnViewLocation() const
{
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}


void AFPSPlayer::Fire()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->CurrentMagazineAmmo > 0 && !CurrentWeapon->IsReloading)
		{
			if (CanFire)
			{
				if (CurrentWeapon->FireMontage) {
					if (CurrentWeapon == Revolver)
					{
						CanFire = false;
					}
					GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->FireMontage);
				}
				CurrentWeapon->FireShot(this);
				if (CurrentWeapon != Revolver)
				{
					Reload();
				}
			}
		}
		else
		{
			Reload();
		}
	}
}

//void AFPSPlayer::Reload()
//{
//	GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->ReloadMontage);
//	
//	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadTime);
//	
//	CanFire = true;
//}

void AFPSPlayer::Reload()
{
	if (CurrentWeapon)
	{
		if (!CurrentWeapon->IsReloading && CurrentWeapon->CurrentAmmo > 0 && CurrentWeapon->CurrentMagazineAmmo < CurrentWeapon->MagazineCapacity)
		{
			//GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->ReloadMontage);
			//IsReloading = true;
			CurrentWeapon->ReloadWithTimer();
			//CanFire = true;
			//IsReloading = false;
			/*GetWorldTimerManager().SetTimer(TimerHandle_ReloadTime, this, &AFPSPlayer::Reload, CurrentWeapon->ReloadWaitTime, false);*/
		}
	}
}

FString AFPSPlayer::GetWeaponName()
{
	if (CurrentWeapon)
	{
		return FString(CurrentWeapon->WeaponName);
	}
	return FString("");
}

FString AFPSPlayer::GetCurrentAmmo()
{
	if (CurrentWeapon)
	{
		return FString("/ " + FString::FromInt(CurrentWeapon->CurrentAmmo));
	}
	return FString("");
}

float AFPSPlayer::GetHealth()
{
	return HealthComp->GetHealth();
}

FString AFPSPlayer::GutCurrentMagazineAmmo()
{
	if (CurrentWeapon)
	{
		return FString(FString::FromInt(CurrentWeapon->CurrentMagazineAmmo));
	}
	return FString("");
}

float AFPSPlayer::GetArmor()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, TEXT("Hit"));
	return (float)HealthComp->GetArmor();
}

void AFPSPlayer::OnHealthChanged(UHealthComponent* HealthComponent, float Health)
{
	if (Health <= 0.f && !bIsDied)
	{
		GetCharacterMovement()->StopMovementImmediately();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bIsDied = true;

	}
}

void AFPSPlayer::Die()
{
	DetachFromControllerPendingDestroy();

	SetLifeSpan(10.f);
}

void AFPSPlayer::Refill()
{
	if (Revolver)
	{
		Revolver->Refill();
	}
	if (Launcher)
	{
		Launcher->Refill();
	}
	if (Grenade)
	{
		Grenade->Refill();
	}
}

void AFPSPlayer::Heal()
{
	APickup* Item = Inventory->FindItemByName(FString("Medkit"));
	if (Item)
	{
		Item->Use_Implementation();
		//RemoveFromInventory(this);
	}
	//HealthComp->Heal(HealingAmount);
}


void AFPSPlayer::EquipRevolver()
{
	if (Revolver)
	{
		CurrentWeapon = Revolver;
		EquipedWeapon = WeaponType::Revolver;
	}
}

void AFPSPlayer::EquipLauncher()
{
	if (Launcher)
	{
		CurrentWeapon = Launcher;
		EquipedWeapon = WeaponType::Launcher;
	}
}

void AFPSPlayer::EquipGrenade()
{
	if (Grenade)
	{
		CurrentWeapon = Grenade;
		EquipedWeapon = WeaponType::Grenade;
	}
}

void AFPSPlayer::SwitchWeapon()
{
	if (EquipedWeapon == WeaponType::Revolver)
	{
		EquipLauncher();
	}
	else
	{
		EquipRevolver();
	}
}

void AFPSPlayer::NextWeapon()
{
	switch (EquipedWeapon)
	{
	case WeaponType::Revolver:
		EquipLauncher();
		break;

	case WeaponType::Launcher:
		EquipGrenade();
		break;

	case WeaponType::Grenade:
		EquipRevolver();
		break;
	}
}

void AFPSPlayer::PreviousWeapon()
{
	switch (EquipedWeapon)
	{
	case WeaponType::Revolver:
		EquipGrenade();
		break;

	case WeaponType::Launcher:
		EquipRevolver();
		break;

	case WeaponType::Grenade:
		EquipLauncher();
		break;
	}
}

void AFPSPlayer::SwitchToFPS()
{
	CameraBoom->TargetArmLength = 0.f;
	CameraBoom->SocketOffset.Y = 0.f;
}

void AFPSPlayer::SwitchToTPS()
{
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset.Y = 60.f;
}

void AFPSPlayer::SwitchCamera()
{
	if (!CameraSwitch)
	{
		SwitchToTPS();
		CameraSwitch = true;
	}
	else
	{
		SwitchToFPS();
		CameraSwitch = false;
	}
}

