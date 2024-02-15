// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_Detect.h"
#include "AIControllerBase.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GrowingHero/UnitBase.h"

UService_Detect::UService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	AUnitBase* SelfUnit = Cast<AUnitBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AAIControllerBase::Key_SelfUnit));
	if (SelfUnit == nullptr)
	{
		return;
	}
	if (SelfUnit->getUnitState() == EUNIT_STATE::E_Dead)
	{
		OwnerComp.GetAIOwner()->StopMovement();
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIControllerBase::Key_Target, nullptr);
		return;
	}
	if (SelfUnit->getUnitState() == EUNIT_STATE::E_Attack || SelfUnit->getUnitState() == EUNIT_STATE::E_UnderAttack)
	{
		return;
	}
	

	// �� �����
	UWorld* World = SelfUnit->GetWorld();
	// UWorld* World = AAIControllerBase::SelfUnit->GetWorld();
	if (World == nullptr) 
		return;

	FVector Center = SelfUnit->GetActorLocation();
	// FVector Center = AAIControllerBase::SelfUnit->GetActorLocation();
	float DetectRadius = 300.0f;

	// 300�� �������� ���� ��ü�� ���� ������Ʈ�� �����Ѵ�.
	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, SelfUnit);
	//FCollisionQueryParams CollisionQueryParam(NAME_None, false, AAIControllerBase::SelfUnit);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// ������Ʈ�� ������ �Ǹ�, �� ������Ʈ�� Character���� �˻��Ѵ�.
	if (bResult)
	{
		for (FOverlapResult OverlapResult : OverlapResults)
		{
			AUnitBase* Target = Cast<AUnitBase>(OverlapResult.GetActor());
			if (Target && Target->getUnitType() == m_eTargetType)
			{
				// Target�̸�, �����忡 �����Ѵ�.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIControllerBase::Key_Target, Target);
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIControllerBase::Key_Target, nullptr);
	}
}