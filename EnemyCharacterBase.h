// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class HIRS_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Perception")
		class UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Perception")
		class UAISenseConfig_Sight* SenseConfig_Sight;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Weapon")
		class UWeaponObject* CurrentWeapon;

	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
		class UHealthComponent* HealthComp;

	UFUNCTION()
	void OnHealthChanged(UHealthComponent* HealthComponent, float Health);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthComponent")
		bool bIsDied;

	UFUNCTION(BlueprintCallable)
		void Die();

};
