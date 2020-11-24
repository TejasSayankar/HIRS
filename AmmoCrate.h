// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "AmmoCrate.generated.h"

/**
 * 
 */
UCLASS()
class HIRS_API AAmmoCrate : public AInteractable
{
	GENERATED_BODY()
	
protected:
	

public:

	AAmmoCrate();

	void Interact_Implementation(class AFPSPlayer* Player) override;
};
