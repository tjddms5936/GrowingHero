// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_StatWindow.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"

UUMG_StatWindow::UUMG_StatWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UUMG_StatWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
}

void UUMG_StatWindow::NativeConstruct()
{
	Super::NativeConstruct();
}
