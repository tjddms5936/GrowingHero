// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "CombatComponent.h"
#include "MyCharacterController.h"
#include "MyCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	m_eCharType = ECharType::E_Enemy;
}

void AEnemyCharacter::DieEvent()
{
	DropItem();
	AddEXP();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
}


void AEnemyCharacter::NotifyActorOnClicked(FKey PressedButton)
{
	if (m_pMyController && getUnitState() != EUNIT_STATE::E_Dead)
	{
		if (m_pMyController->getClickedActor() != this)
		{
			m_pMyController->setClickedActor(this);
			if (m_pMyController->ED_EnemyOutlineOff.IsBound() == true)
				m_pMyController->ED_EnemyOutlineOff.Execute(); // 이전에 클릭 한 적이 있다면 바인딩 되어있을 것임.

			GetMesh()->SetRenderCustomDepth(true);
			m_pMyController->ED_EnemyOutlineOff.BindUFunction(this, FName("OutlineOff"));
		}
		else
		{
			m_pMyController->AttackAgain();
		}
			
	}
}

AUnitBase* AEnemyCharacter::getTarget()
{
	return CombatComponent->getTarget();
}

void AEnemyCharacter::setTargetClear()
{
	CombatComponent->setTargetClear();
}

void AEnemyCharacter::DropItem()
{
	FActorSpawnParameters spawnParams{};
	FRotator rotator{};
	FVector location{};

	rotator =GetActorRotation();

	int32 nItemDropCount{};
	nItemDropCount = FMath::RandRange(0, m_DropItemList.Num());

	location = GetActorLocation();

	int nCurDropItemCount{};
	for (auto item : m_DropItemList)
	{
		if (nCurDropItemCount == nItemDropCount)
			return;

		GetWorld()->SpawnActor<AInterfaceProp_Inventory>(item.Get(), location, rotator, spawnParams);
		location.X += FMath::RandRange(-100.f, 100.f);
		location.Y += FMath::RandRange(-100.f, 100.f);
		nCurDropItemCount++;
	}
}

void AEnemyCharacter::AddEXP()
{
	if (m_pMyHero->getUnitState() != EUNIT_STATE::E_Dead)
	{
		m_pMyHero->GainEXT(getUnitStat()->CurEXP);
	}
}

void AEnemyCharacter::MediateAttackRange(float fAddRangeAmount)
{
	Super::MediateAttackRange(fAddRangeAmount);
}

void AEnemyCharacter::OutlineOff()
{
	// 윤곽선 꺼주고 연결되어있는 바인딩을 끊어줘야 한다.
	// 무슨말이냐면, 이 함수가 호출되었다는 건 다른 적을 클릭해서 기존의 적의 윤곽선을 끄기 위해 불려진것
	// 즉, 이전 클릭 된 적이 이 함수를 바인딩 한 상태이기 때문에, 이걸 끊어줘야 한다. 
	GetMesh()->SetRenderCustomDepth(false);
	m_pMyController->ED_EnemyOutlineOff.Unbind();
}


