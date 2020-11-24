// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "FPSPlayer.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	InteractableHelpText = "Press E to Interact";

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AInteractable::Interact_Implementation(AFPSPlayer* Player)
{

}

