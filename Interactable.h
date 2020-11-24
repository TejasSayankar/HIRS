// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class HIRS_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
		FString InteractableHelpText;

	virtual void Interact_Implementation(class AFPSPlayer* Player);


};
