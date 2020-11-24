// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSPlayer.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8 {
	Revolver = 1 UMETA(DisplayName = "Revolver"),
	Launcher = 2 UMETA(DisplayName = "Launcher"),
	Grenade = 3 UMETA(DisplayName = "Grenade"),
	None = 0 UMETA(DisplayName = "None"),
};

UCLASS()
class HIRS_API AFPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSPlayer();


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* CameraBoom;

	class AInteractable* CurrentInteractable;

	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
		class UWeaponObject* Revolver;

	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
		class UWeaponObject* Launcher;

	UPROPERTY(EditAnywhere, Instanced, Category = "Weapon")
		class UWeaponObject* Grenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<WeaponType> EquipedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* StimuliSourceComp;

	bool CameraSwitch;

	void SetupStimulus();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Jumping();

	void StopJump();

	void Interact();

	void CheckForInteractables();

	void EquipRevolver();

	void EquipLauncher();

	void EquipGrenade();

	void SwitchWeapon();

	void NextWeapon();

	void PreviousWeapon();

	void SwitchToFPS();

	void SwitchToTPS();

	void SwitchCamera();


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactable")
		FString HelpText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Objective")
		FString ObjectiveText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class UWeaponObject* CurrentWeapon;

	//TSubclassOf<UAISense_Sight> AISense_SightClass;

	void Fire();

	FTimerHandle TimerHandle_ReloadTime;

	void Reload();

	void ReloadWithTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		bool CanFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		bool IsReloading;

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GetWeaponName();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GetCurrentAmmo();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		float GetHealth();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GutCurrentMagazineAmmo();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		float GetArmor();

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComponent, float Health);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthComponent")
		bool bIsDied;

	UFUNCTION(BlueprintCallable)
		void Die();

	void Refill();

	void Heal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
		int ObjectiveCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		class UHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
		class UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameOver")
		bool bIsGameOver;
};
