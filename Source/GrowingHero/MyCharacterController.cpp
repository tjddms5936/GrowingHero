// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterController.h"

#include "InventoryComponent.h"
#include "HotKeyComponent.h"
#include "EquipmentComponent.h"
#include "SkillComponent.h"
#include "CombatComponent.h"
#include "StatComponent.h"

#include "UnitBase.h"
#include "EnemyCharacter.h"
#include "MyCharacter.h"
#include "InterfaceProp.h"

#include "UMG/UMG_InventoryFrame.h"
#include "UMG/UMG_EquipmentFrame.h"
#include "UMG/UMG_SkillFrame.h"
#include "UMG/UMG_StatFrame.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "AIController.h"


AMyCharacterController::AMyCharacterController() :
	bClickMouse{},
	m_fInterfaceRagne{}
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	InventorySystemComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventorySystemComponent"));
	HotKeyComponent = CreateDefaultSubobject<UHotKeyComponent>(TEXT("HotKeyComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

	m_fInterfaceRagne = 10.f;
}

void AMyCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AMyCharacterController::InputClickPressed);
	InputComponent->BindAction("MouseLeftClick", IE_Released, this, &AMyCharacterController::InputClickReleased);
	InputComponent->BindAction("InventoryToggle", IE_Pressed, this, &AMyCharacterController::InventoryToggle);
	InputComponent->BindAction("EquipmentWindowToggle", IE_Pressed, this, &AMyCharacterController::EquipmentWindowToggle);
	InputComponent->BindAction("SkillWindowToggle", IE_Pressed, this, &AMyCharacterController::SkillWindowToggle);
	InputComponent->BindAction("StatWindowToggle", IE_Pressed, this, &AMyCharacterController::StatWindowToggle);
	
	DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const EKEY);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_Del", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_Del);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_Ins", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_Ins);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_End", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_End);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_Home", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_Home);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_PgUp", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_PgUp);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_PgDown", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_PgDown);

	
}



void AMyCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bClickMouse)
	{
		if (m_pClickedProp)
		{
			// 위 두가지 조건이 만족하면 보간 시작 : 내가 적을 바라보는 보간 
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), m_pClickedProp->GetActorLocation());
			FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
			// FRotator LookAtYaw = GetLookAtRotationYaw(DestActor->GetActorLocation());
			FRotator InterpRotation = FMath::RInterpTo(GetPawn()->GetActorRotation(), LookAtRotationYaw, DeltaTime, 15.f);

			GetPawn()->SetActorRotation(InterpRotation);
			GetMousePosition(m_fTmpMousePosX, m_fTmpMousePosY);
			if (FMath::Abs(m_fMousePosX - m_fTmpMousePosX) > 100 || FMath::Abs(m_fMousePosY - m_fTmpMousePosY) > 100)
			{
				m_pClickedProp = nullptr;
			}
		}

		MoveToMouseCursor();
	}
}

void AMyCharacterController::BeginPlay()
{
	Super::BeginPlay();
	m_pMyHero = Cast<AMyCharacter>(GetPawn());
	SkillComponent->setOwnerUnit(m_pMyHero);
	SkillComponent->setOwnerController(this);

}


void AMyCharacterController::InputClickPressed()
{
	bClickMouse = true;
	GetMousePosition(m_fMousePosX, m_fMousePosY);
}

void AMyCharacterController::InputClickReleased()
{
	bClickMouse = false;
}

void AMyCharacterController::InventoryToggle()
{
	if (!InventorySystemComponent->InventoryFrame->getToggleState())
	{
		InventorySystemComponent->InventoryFrame->Setup();
		return;
	}
	InventorySystemComponent->InventoryFrame->TearDown();
}

void AMyCharacterController::EquipmentWindowToggle()
{
	if (!EquipmentComponent->EquipmentFrame->getToggleState())
	{
		EquipmentComponent->EquipmentFrame->Setup();
		return;
	}
	EquipmentComponent->EquipmentFrame->TearDown();
}

void AMyCharacterController::SkillWindowToggle()
{
	if (!SkillComponent->SkillFrame->getToggleState())
	{
		SkillComponent->SkillFrame->Setup();
		return;
	}
	SkillComponent->SkillFrame->TearDown();
}

void AMyCharacterController::StatWindowToggle()
{
	if (!StatComponent->StatFrame->getToggleState())
	{
		StatComponent->StatFrame->Setup();
		return;
	}
	StatComponent->StatFrame->TearDown();
}

void AMyCharacterController::HotKeyPressed(EKEY eKey)
{
	if (HotKeyComponent->m_arHotKey[(int)eKey] == nullptr)
		return;

	HotKeyComponent->m_arHotKey[(int)eKey]->Activate();
}

AActor* AMyCharacterController::getClickedActor()
{
	return m_pClickedProp;
}

void AMyCharacterController::AttackAgain()
{
	if (m_pClickedProp == nullptr || m_pMyHero == nullptr)
		return;


	if (!m_pClickedProp->ActorHasTag("Enemy"))
		return;
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(m_pClickedProp);

	// AttackEnemy() 함수 내부에서 범위 체크함
	m_pMyHero->AttackEnemy(pEnemy);
}

FVector AMyCharacterController::getMouseLocation()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	return Hit.Location;
}

// ======================================= 네비 관련 함수 ====================================
void AMyCharacterController::MoveToMouseCursor()
{
	// ﻿마우스 커서 아래에 레이 트레이스를 쏘고
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);
	
	
	// 차단 충돌여부의 결과를 보고
	if (Hit.bBlockingHit)
	{
		if (m_pMyHero == nullptr)
			return;

		// 마우스 눌린 곳으로 이동하기 위해 함수 호출
		if (Hit.GetActor()->ActorHasTag("Enemy"))
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor());
			if (Enemy)
				SetNewDestination(Hit.ImpactPoint, Hit.GetActor(), m_pMyHero->getUnitStat()->AttackRange - 50.f, true);
		}
		else
		{
			SetNewDestination(Hit.ImpactPoint, Hit.GetActor(), m_fInterfaceRagne, false);
		}
	}
}


UPathFollowingComponent* AMyCharacterController::InitNaviControl()
{
	UPathFollowingComponent* PathFollowingComp = nullptr;

	PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
	if (PathFollowingComp == nullptr)
	{
		PathFollowingComp = NewObject<UPathFollowingComponent>(this);
		PathFollowingComp->RegisterComponentWithWorld(GetWorld());
		PathFollowingComp->Initialize();
	}

	return PathFollowingComp;
}

void AMyCharacterController::SetNewDestination(const FVector DestLocation, AActor* DestActor, float DstDistance, bool IsEnemy)
{
	if (m_pMyHero == nullptr)
		return;

	if (m_pMyHero->getUnitState() != EUNIT_STATE::E_Idle)
	{
		return;
	}

	// 목적지와 내 캐릭터 사이의 거리 측정
	float const Distance = FVector::Dist(DestLocation, m_pMyHero->GetActorLocation()) - 60.f;
	// 120 언리얼 유닛 이상이면 이동
	if (Distance <= DstDistance)
	{
		if(m_pMyHero->getUnitState() == EUNIT_STATE::E_Idle)
		return;
	}
	

	UNavigationSystemV1* NavSys = this ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld()) : nullptr;
	if (NavSys == nullptr || m_pMyHero == nullptr)
	{
		return;
	}

	UPathFollowingComponent* PFollowComp = InitNaviControl();
	if (!PFollowComp->IsPathFollowingAllowed())
	{
		return;
	}

	const bool bAlreadyAtGoal = PFollowComp->HasReached(DestLocation, EPathFollowingReachMode::OverlapAgent);
	if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
		m_pMyHero->setUnitState(EUNIT_STATE::E_Idle);
	}
	if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
	{
		PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
		m_pMyHero->setUnitState(EUNIT_STATE::E_Idle);
	}

	if (bAlreadyAtGoal)
	{
		PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		m_pMyHero->setUnitState(EUNIT_STATE::E_Idle);
	}
	else
	{
		const FVector AgentNavLocation = GetNavAgentLocation();
		const ANavigationData* NavData = NavSys->GetNavDataForProps(GetNavAgentPropertiesRef(), AgentNavLocation);
		if (NavData)
		{
			FPathFindingQuery Query(this, *NavData, AgentNavLocation, DestLocation);
			FPathFindingResult Result = NavSys->FindPathSync(Query);
			if (Result.IsSuccessful())
			{
				m_pMyHero->setUnitState(EUNIT_STATE::E_MovingToActor);
				if (IsEnemy)
				{
					m_pMyHero->setUnitState(EUNIT_STATE::E_MovingToEnemy);
				}
				FAIMoveRequest Req = FAIMoveRequest(DestLocation);
				Req.SetAcceptanceRadius(DstDistance);
				PFollowComp->RequestMove(Req, Result.Path);

				m_pMyHero->setUnitState(EUNIT_STATE::E_Idle);
			}
			else if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle)
			{
				PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
				m_pMyHero->setUnitState(EUNIT_STATE::E_Idle);
			}
		}
	}
}