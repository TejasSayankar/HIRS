// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoCrate.h"
#include "FPSPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

AAmmoCrate::AAmmoCrate()
{
	
}

void AAmmoCrate::Interact_Implementation(AFPSPlayer* Player)
{
	if (Player)
	{
		Player->Refill();
		Destroy();
	}
}
