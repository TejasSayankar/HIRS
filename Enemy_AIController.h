// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class HIRS_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_AIController();

	void BeginPlay() override;
	void OnPossess(APawn* const MyPawn) override;

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName TargetLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName CanSeePlayerKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName PlayerKey;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsPlayerDetected;
		

protected:
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	class UBlackboardComponent* BlackboardComponent;

	class UAISenseConfig_Sight* SenseConfig_Sight;
public:
	//class UAIPerceptionComponent* AIPerceptionComp;
	UFUNCTION(BlueprintCallable)
		FORCEINLINE UBlackboardComponent* GetBlackboard() const { return BlackboardComponent; }

private:
	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

	/*UFUNCTION()
		void OnUpdated(const TArray<AActor*>& UpdatedActors);*/
	/*void SetupPerceptionSystem();*/
};
