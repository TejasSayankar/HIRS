// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSPlayer.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"




AEnemy_AIController::AEnemy_AIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	GetPerceptionComponent();
	//AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp")));
	SenseConfig_Sight->SightRadius = 3000.f;
	SenseConfig_Sight->LoseSightRadius = SenseConfig_Sight->SightRadius + 500.f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 80.f;
	SenseConfig_Sight->SetMaxAge(5.0f);
	SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = 200.0f;
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(SenseConfig_Sight->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_AIController::OnTargetDetected);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemy_AIController::OnUpdated);
	GetPerceptionComponent()->ConfigureSense(*SenseConfig_Sight);
	

	TargetLocationKey = "TargetLocation";
	CanSeePlayerKey = "CanSeePlayer";
	PlayerKey = "Player";

	bIsPlayerDetected = false;

}

void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
	/*if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		if (BehaviorTreeComponent)
		{
			BehaviorTreeComponent->StartTree(*BehaviorTree);
		}
	}*/
	
}

void AEnemy_AIController::OnPossess(APawn* const MyPawn)
{
	Super::OnPossess(MyPawn);

	auto const AICharacter = Cast<AEnemyCharacterBase>(MyPawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree)
		{
			RunBehaviorTree(AICharacter->BehaviorTree);
			if (BlackboardComponent)
			{
				BlackboardComponent->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			}
			if (BehaviorTreeComponent)
			{
				BehaviorTreeComponent->StartTree(*(AICharacter->BehaviorTree));
			}
		}
		
	}
	
}

//UBlackboardComponent* AEnemy_AIController::GetBlackboard() const
//{
//	return BlackboardComponent;
//}

void AEnemy_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	auto const PlayerCharacter = Cast<AFPSPlayer>(Actor);
	if (PlayerCharacter)
	{
		bIsPlayerDetected = true;
		//GetBlackboard()->SetValueAsObject(PlayerKey, PlayerCharacter);
		GetBlackboard()->SetValueAsObject(PlayerKey, Actor);
		GetBlackboard()->SetValueAsBool(CanSeePlayerKey, Stimulus.WasSuccessfullySensed());
	}
}

