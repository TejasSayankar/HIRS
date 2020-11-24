// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
//#include "GameFramework/Pawn.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
//#include "BehaviorTree/BehaviorTreeTypes.h"
//#include "UObject/UObjectGlobals.h"
#include "BTTask_FindTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class HIRS_API UBTTask_FindTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindTargetLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float SearchRadius = 1500.f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool NeedRandom;
};
