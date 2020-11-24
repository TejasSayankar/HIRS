// Fill out your copyright notice in the Description page of Project Settings.


#include "Medkit.h"
#include "FPSPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"

AMedkit::AMedkit()
{
	HealingAmount = 0.f;
}


void AMedkit::Use_Implementation()
{
	AFPSPlayer* const Player = Cast<AFPSPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->HealthComp->Heal(HealingAmount))
		{
			//Player->Inventory->RemoveFromInventory(this);
			Destroy();
		}
	}
	
}

void AMedkit::Interact_Implementation(AFPSPlayer* Player)
{
	Player->HealthComp->Heal(HealingAmount);
	Destroy();
}
