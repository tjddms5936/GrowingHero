// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "CombatComponent.h"
#include "MyCharacterController.h"
#include "MyCharacter.h"

#include "Kismet/GameplayStatics.h"
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

