// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */

UCLASS()
class HIRS_API APickup : public AInteractable
{
	GENERATED_BODY()

	enum EPickupType : uint8
	{
		Pickup_Item,
		Pickup_Weapon,
	};

public:
	APickup();

public:

	UPROPERTY(EditAnywhere, Category = "Pickup")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		UTexture2D* ItemThumbnail;

	UPROPERTY(EditAnywhere, Category = "Pickup")
		uint8 PickupType;

	virtual void Interact_Implementation(class AFPSPlayer* Player) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void Interact(class AFPSPlayer* Player);

	virtual void Use_Implementation();

	void OnPickedUp();
};
