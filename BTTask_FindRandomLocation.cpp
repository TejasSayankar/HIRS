// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GameFramework/Character.h"
//#include "GameFramework/Pawn.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"
//#include "BehaviorTree/BehaviorTreeTypes.h"
//#include "UObject/UObjectGlobals.h"


UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	auto const Controller = Cast<AEnemy_AIController>(Owner.GetAIOwner());
	
	if (Controller)
	{
		auto const Character = Cast<ACharacter>(Controller->GetPawn());
		if (Character)
		{
			//FVector const Origin = Character->GetActorLocation();
			FNavLocation Loc;

			UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

			if (NavSystem->GetRandomPointInNavigableRadius(Controller->GetBlackboard()->GetValueAsVector(Controller->TargetLocationKey), SearchRadius, Loc, nullptr))
			{
				Controller->GetBlackboard()->SetValueAsVector(Controller->TargetLocationKey, Loc.Location);
			}

			FinishLatentTask(Owner, EBTNodeResult::Type::Succeeded);

			//return EBTNodeResult::Succeeded;
			return EBTNodeResult::Succeeded;
		}
		FinishLatentTask(Owner, EBTNodeResult::Type::Failed);
		return EBTNodeResult::Failed;
	}
	FinishLatentTask(Owner, EBTNodeResult::Type::Failed);
	return EBTNodeResult::Failed;
}
	