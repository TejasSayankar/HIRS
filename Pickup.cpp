// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "FPSPlayer.h"
#include "InventoryComponent.h"

APickup::APickup()
{
	ItemName = "Enter the Item Name";
	InteractableHelpText = "Press E to Pickup";

}

void APickup::Interact_Implementation(AFPSPlayer* Player)
{
	Player->Inventory->AddItemToInventory(this);
	Interact(Player);
	OnPickedUp();
	
}

void APickup::Use_Implementation()
{
	//Destroy();
}

void APickup::OnPickedUp()
{
	Destroy();
}
