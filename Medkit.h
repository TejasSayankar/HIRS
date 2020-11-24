// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Medkit.generated.h"

/**
 * 
 */
UCLASS()
class HIRS_API AMedkit : public APickup
{
	GENERATED_BODY()
	
public:
	AMedkit();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float HealingAmount;

public:
	void Use_Implementation() override;

	void Interact_Implementation(AFPSPlayer* Player) override;
};
