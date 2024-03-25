// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterController.h"

#include "InventoryComponent.h"
#include "HotKeyComponent.h"
#include "EquipmentComponent.h"
#include "SkillComponent.h"
#include "StatComponent.h"
#include "CameraTransparencyComponent.h"

#include "EnemyCharacter.h"
#include "MyCharacter.h"
#include "MainGameInstance.h"
#include "QuestMGR.h"

#include "UMG//UMG_InventoryFrame.h"
#include "UMG/UMG_EquipmentFrame.h"
#include "UMG/UMG_SkillFrame.h"
#include "UMG/UMG_StatFrame.h"
#include "UMG/UMG_GameHUD.h"
#include "UMG/UMG_HotKeyWindow.h"

#include "Kismet/KismetMathLibrary.h"

AMyCharacterController::AMyCharacterController() :
	bClickMouse{},
	m_fInterfaceRagne{},
	m_arOpenedFrame{}
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	InventorySystemComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventorySystemComponent"));
	HotKeyComponent = CreateDefaultSubobject<UHotKeyComponent>(TEXT("HotKeyComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	CameraTransparencyComponent = CreateDefaultSubobject<UCameraTransparencyComponent>(TEXT("CameraTransparencyComponent"));

	m_fInterfaceRagne = 10.f;

	DefaultMouseCursor = EMouseCursor::Default;
	CurrentMouseCursor = EMouseCursor::Default;
}

void AMyCharacterController::SetupInputComponent()
{
	EnableInput(this);
	Super::SetupInputComponent();
	
	if (InputComponent)
	{
		InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AMyCharacterController::InputClickPressed);
		InputComponent->BindAction("MouseLeftClick", IE_Released, this, &AMyCharacterController::InputClickReleased);
		InputComponent->BindAction("InventoryToggle", IE_Pressed, this, &AMyCharacterController::InventoryToggle);
		InputComponent->BindAction("EquipmentWindowToggle", IE_Pressed, this, &AMyCharacterController::EquipmentWindowToggle);
		InputComponent->BindAction("SkillWindowToggle", IE_Pressed, this, &AMyCharacterController::SkillWindowToggle);
		InputComponent->BindAction("StatWindowToggle", IE_Pressed, this, &AMyCharacterController::StatWindowToggle);
		InputComponent->BindAction("OpendFrameTearDown", IE_Pressed, this, &AMyCharacterController::OpenedFrameTearDownPressed);
	
		DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const EKEY);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_1", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_1);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_2", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_2);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_3", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_3);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_4", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_4);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_5", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_5);
		InputComponent->BindAction<FCustomInputDelegate>("HOTKEY_6", IE_Pressed, this, &AMyCharacterController::HotKeyPressed, EKEY::E_6);

		InputComponent->BindAction("NPC_Conversation", IE_Pressed, this, &AMyCharacterController::NPC_ConversationKeyPressed);
		

		InputComponent->BindAxis("MoveForward", this, &AMyCharacterController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMyCharacterController::MoveRight);
	}
}



void AMyCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AMyCharacterController::BeginPlay()
{
	FInputModeGameAndUI InputModeData;
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	m_pMyHero = Cast<AMyCharacter>(GetPawn());
	
	UMainGameInstance* GameIns = Cast<UMainGameInstance>(GetGameInstance());
	if (IsValid(GameIns) && IsValid(GameIns->m_QuestMGR))
	{
		GameIns->m_QuestMGR->m_pMyController = this;
		GameIns->m_QuestMGR->m_pMyHero = m_pMyHero;
		GameIns->m_QuestMGR->m_world = GetWorld();
	}

	InitComponentSetting();
	LoadUMGSetting();
	// 블루프린트의 BeginPlay를 위의 작업 이후에 호출하기 위함.
	Super::BeginPlay();
}


void AMyCharacterController::InputClickPressed()
{
	bClickMouse = true;
}

void AMyCharacterController::InputClickReleased()
{
	bClickMouse = false;
}

void AMyCharacterController::InventoryToggle()
{
	InventoryFrame->ToggleFrame();
	if (InventoryFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(InventoryFrame);
	}
}

void AMyCharacterController::EquipmentWindowToggle()
{
	EquipmentFrame->ToggleFrame();
	if (EquipmentFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(EquipmentFrame);
	}
}

void AMyCharacterController::SkillWindowToggle()
{
	SkillFrame->ToggleFrame();
	if (SkillFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(SkillFrame);
	}
}

void AMyCharacterController::StatWindowToggle()
{
	StatFrame->ToggleFrame();
	if (StatFrame->m_bOpenFrame)
	{
		m_arOpenedFrame.Enqueue(StatFrame);
	}
}

void AMyCharacterController::HotKeyPressed(EKEY eKey)
{
	HotKeyComponent->ActivateHotKey(eKey);
}

void AMyCharacterController::NPC_ConversationKeyPressed()
{
	// Bind, UnBind는 NPC가 처리하고 여기서는 키 누르면 연결된 바인딩 함수 호출만 한다.
	if (ED_NPC_Conversation.IsBound() == true)
	{
		ED_NPC_Conversation.Broadcast(); // NPC 범위 내에 들어가있다면 NPC가 바인딩 했을 것이다.
	}
}

void AMyCharacterController::OpenedFrameTearDownPressed()
{
	UUMG_Frame* pTmpFrame{};
	while (!m_arOpenedFrame.IsEmpty())
	{
		m_arOpenedFrame.Dequeue(pTmpFrame);
		if (pTmpFrame->m_bOpenFrame)
		{
			pTmpFrame->ToggleFrame();
			break;
		}
	}
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

void AMyCharacterController::InitComponentSetting()
{
	if (InventorySystemComponent)
		InventorySystemComponent->init();
	if (EquipmentComponent)
		EquipmentComponent->init();
	if (SkillComponent)
		SkillComponent->init();
	if (StatComponent)
		StatComponent->init(m_pMyHero);
	if (CameraTransparencyComponent)
		CameraTransparencyComponent->init(Cast<USpringArmComponent>(m_pMyHero->GetComponentByClass(USpringArmComponent::StaticClass())),
			Cast<UCameraComponent>(m_pMyHero->GetComponentByClass(UCameraComponent::StaticClass())),
			Cast<UCapsuleComponent>(m_pMyHero->GetComponentByClass(UCapsuleComponent::StaticClass())),
			m_pMyHero);
}

void AMyCharacterController::LoadUMGSetting()
{
	BackGround = CreateWidget<UUserWidget>(GetWorld(), BackGroundClass, FName(BackGroundClass->GetName()));
	if (BackGround)
	{
		BackGround->AddToViewport();
		BackGround->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}


	InventoryFrame = CreateWidget<UUMG_InventoryFrame>(GetWorld(), InventoryFrameClass, FName(InventoryFrameClass->GetName()));
	if (InventoryFrame)
	{
		InventoryFrame->AddToViewport();
		InventoryFrame->SetVisibility(ESlateVisibility::Hidden);
		InventoryFrame->init();
	}

	EquipmentFrame = CreateWidget<UUMG_EquipmentFrame>(GetWorld(), EquipmentFrameClass, FName(EquipmentFrameClass->GetName()));
	if (EquipmentFrame)
	{
		EquipmentFrame->AddToViewport();
		EquipmentFrame->SetVisibility(ESlateVisibility::Hidden);
		EquipmentFrame->init();
	}

	SkillFrame = CreateWidget<UUMG_SkillFrame>(GetWorld(), SkillFrameClass, FName(SkillFrameClass->GetName()));
	if (SkillFrame)
	{
		SkillFrame->AddToViewport();
		SkillFrame->SetVisibility(ESlateVisibility::Hidden);
		SkillFrame->init();
	}

	StatFrame = CreateWidget<UUMG_StatFrame>(GetWorld(), StatFrameClass, FName(StatFrameClass->GetName()));
	if (StatFrame)
	{
		StatFrame->AddToViewport();
		StatFrame->SetVisibility(ESlateVisibility::Hidden);
		StatFrame->init();
	}


	GameHUD = CreateWidget<UUMG_GameHUD>(GetWorld(), GameHUDClass, FName(GameHUDClass->GetName()));
	if (GameHUD)
	{
		GameHUD->AddToViewport();
		GameHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		GameHUD->SetRenderOpacity(0.f);
		GameHUD->init();
	}

	HotKeyWindow = CreateWidget<UUMG_HotKeyWindow>(GetWorld(), HotKeyWindowClass, FName(HotKeyWindowClass->GetName()));
	if (HotKeyWindow)
	{
		HotKeyWindow->AddToViewport();
		HotKeyWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HotKeyWindow->SetRenderOpacity(0.f);
		HotKeyWindow->init();
	}
}

AActor* AMyCharacterController::getClickedActor()
{
	return m_pClickedProp;
}

void AMyCharacterController::AttackAgain()
{
	if (!IsValid(m_pClickedProp) || !IsValid(m_pMyHero) || m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead)
		return;
	
	if (!m_pClickedProp->ActorHasTag("Enemy"))
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("AttackAgain"));
	AEnemyCharacter* pEnemy = Cast<AEnemyCharacter>(m_pClickedProp);
	m_pMyHero->AttackEnemy(pEnemy);
}

FVector AMyCharacterController::getMouseLocation()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	return Hit.Location;
}

void AMyCharacterController::setClickedActor(AActor* pActor)
{
	if (m_pMyHero->getUnitState() == EUNIT_STATE::E_Dead)
	{
		m_pClickedProp = nullptr;
		return;
	}
	m_pClickedProp = pActor;
}

void AMyCharacterController::CharLevelUp()
{
	StatComponent->AddStatPoint(5);
	StatComponent->StatUp(EStatName::E_Level, 1.f);
	StatComponent->UpdateStat();

	SkillComponent->AddSkillPoint(1);
}

void AMyCharacterController::InventoryFrameRefresh(EItemType eItemType)
{
	InventoryFrame->UpdateInventoryWindow(eItemType);
}

void AMyCharacterController::LoadInventoryHotKey(const TMap<int32, int32>& mapData)
{
	InventoryFrame->LoadInventoryHotKey(mapData);
}

void AMyCharacterController::EquipmentFrameRefresh()
{
	EquipmentFrame->UpdateEquipmentWindow();
}

void AMyCharacterController::SkillFrameRefresh()
{
	SkillFrame->UpdateSkillWindow();
}

void AMyCharacterController::LoadSkillHotKey(const TMap<int32, int32>& mapData)
{
	SkillFrame->LoadSkillHotKey(mapData);
}

void AMyCharacterController::LoadHotKeyInfo()
{
	HotKeyWindow->LoadHotKey();
}
