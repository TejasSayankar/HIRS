// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "EnemyCharacterBase.h"
#include "Enemy_AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& Owner, uint8* NodeMemory)
{
	AEnemyCharacterBase* const EnemyAI = Cast<AEnemyCharacterBase>(Owner.GetAIOwner()->GetPawn());
	if (EnemyAI)
	{
		EnemyAI->Fire();
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Attacking"));
		FinishLatentTask(Owner, EBTNodeResult::Type::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Not Attacking"));
	FinishLatentTask(Owner, EBTNodeResult::Type::Failed);
	return EBTNodeResult::Failed;
}
