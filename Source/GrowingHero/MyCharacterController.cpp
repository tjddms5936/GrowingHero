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
#include "Widgets/SWidget.h"

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

	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
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
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_1", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_1);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_2", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_2);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_3", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_3);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_4", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_4);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_5", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_5);
	InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_6", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_6);

	InputComponent->BindAxis("MoveForward", this, &AMyCharacterController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyCharacterController::MoveRight);
}



void AMyCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
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
	if (!IsValid(InventorySystemComponent) || !IsValid(InventorySystemComponent->InventoryFrame))
		return;

	if (!InventorySystemComponent->InventoryFrame->getToggleState())
	{
		InventorySystemComponent->InventoryFrame->Setup();
		return;
	}
	InventorySystemComponent->InventoryFrame->TearDown();
}

void AMyCharacterController::EquipmentWindowToggle()
{
	if (!IsValid(EquipmentComponent) || !IsValid(EquipmentComponent->EquipmentFrame))
		return;

	if (!EquipmentComponent->EquipmentFrame->getToggleState())
	{
		EquipmentComponent->EquipmentFrame->Setup();
		return;
	}
	EquipmentComponent->EquipmentFrame->TearDown();
}

void AMyCharacterController::SkillWindowToggle()
{
	if (!IsValid(SkillComponent) || !IsValid(SkillComponent->SkillFrame))
		return;

	if (!SkillComponent->SkillFrame->getToggleState())
	{
		SkillComponent->SkillFrame->Setup();
		return;
	}
	SkillComponent->SkillFrame->TearDown();
}

void AMyCharacterController::StatWindowToggle()
{
	if (!IsValid(StatComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("!IsValid(StatComponent)"));
		return;
	}
	if (!IsValid(StatComponent->StatFrame))
	{
		UE_LOG(LogTemp, Warning, TEXT("!IsValid(StatComponent->StatFrame)"));
		return;
	}
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

void AMyCharacterController::MoveForward(float Value)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Attack ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	const FRotator Rotation = m_pMyHero->Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 카메라 전환되면 자연스럽게 캐릭터도 그 방향 봄
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	m_pMyHero->AddMovementInput(Direction, Value);
}

void AMyCharacterController::MoveRight(float Value)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Attack ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pMyHero->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	const FRotator Rotation = m_pMyHero->Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 카메라 전환되면 자연스럽게 캐릭터도 그 방향 봄
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	m_pMyHero->AddMovementInput(Direction, Value);
}

AActor* AMyCharacterController::getClickedActor()
{
	return m_pClickedProp;
}

void AMyCharacterController::AttackAgain()
{
	if (!IsValid(m_pClickedProp) || !IsValid(m_pMyHero))
		return;
	
	if (!m_pClickedProp->ActorHasTag("Enemy"))
		return;
	
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(m_pClickedProp);

	// AttackEnemy() 함수 내부에서 범위 체크함
	UE_LOG(LogTemp, Warning, TEXT("AMyCharacterController::AttackAgain"));
	m_pMyHero->AttackEnemy(pEnemy);
}

FVector AMyCharacterController::getMouseLocation()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	return Hit.Location;
}