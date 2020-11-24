// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Math/Vector.h"


UBTTask_FindTargetLocation::UBTTask_FindTargetLocation()
{
	NodeName = TEXT("Find Target Location");
	NeedRandom = false;
} 

EBTNodeResult::Type UBTTask_FindTargetLocation::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<AEnemy_AIController>(Owner.GetAIOwner());
	//auto const Character = Controller->GetPawn();
	if (Controller)
	{
		if (Player)
		{
			FVector const PlayerLocation = Player->GetActorLocation();

			Controller->GetBlackboard()->SetValueAsVector(Controller->TargetLocationKey, PlayerLocation);

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
