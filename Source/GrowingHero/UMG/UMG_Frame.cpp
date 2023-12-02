// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Frame.h"
#include "GrowingHero/MyCharacterController.h"
#include "Kismet/GameplayStatics.h"

UUMG_Frame::UUMG_Frame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_bOpenFrame{}
{
	m_bOpenFrame = false;
}

void UUMG_Frame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UUMG_Frame::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_Frame::Setup()
{
	if(!IsValid(m_pMyController))
		m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_bOpenFrame = true;
	this->AddToViewport();
	this->bIsFocusable = false;
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetHideCursorDuringCapture(false);
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	m_pMyController->SetInputMode(InputModeData);
	m_pMyController->SetShowMouseCursor(true);

	UGameplayStatics::SpawnSound2D(GetWorld(), SetupSound);
}

void UUMG_Frame::TearDown()
{
	if (!IsValid(m_pMyController))
		m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_bOpenFrame = false;
	this->RemoveFromParent();
	this->bIsFocusable = false;
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(false);
	FInputModeGameAndUI InputModeData2;
	InputModeData2.SetHideCursorDuringCapture(false);
	InputModeData2.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);

	m_pMyController->SetInputMode(InputModeData2);
	m_pMyController->SetShowMouseCursor(true);

	UGameplayStatics::SpawnSound2D(GetWorld(), TearDownSound);
}

bool UUMG_Frame::getToggleState()
{
    return m_bOpenFrame;
}
