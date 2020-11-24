// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	//Health = 100.f;
	//Armor = 100.f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	Health = DefaultHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	if (Armor >= Damage)
	{
		Armor = FMath::Clamp(Armor - Damage, 0.f, DefaultHealth);
	}
	else if (Armor > 0.f)
	{
		float DamageLeft = Damage - Armor;
		Armor = FMath::Clamp(Armor - Damage, 0.f, DefaultHealth);
		Health = FMath::Clamp(Health - DamageLeft, 0.f, DefaultHealth);
	}
	else
	{
		Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);
	}
	
	OnHealthChanged.Broadcast(this, Health);
}

bool UHealthComponent::Heal(float HealingAmount)
{
	if (Health+HealingAmount <= DefaultHealth)
	{
		Health += HealingAmount;
		return true;
	}
	else if(Health <DefaultHealth)
	{
		Health = DefaultHealth;
		return true;
	}
	return false;
}

float UHealthComponent::GetHealth()
{
	return Health;
}

float UHealthComponent::GetArmor()
{
	return Armor;
}



