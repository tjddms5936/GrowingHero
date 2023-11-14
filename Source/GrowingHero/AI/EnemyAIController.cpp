// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "UObject/ConstructorHelpers.h"

const FName AEnemyAIController::Key_HomePos(TEXT("HomePos"));
const FName AEnemyAIController::Key_PatrolPos(TEXT("PatrolPos"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/AI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/AI/BT_Enemy.BT_Enemy"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
