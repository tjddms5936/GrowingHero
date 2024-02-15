// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_HotKeyWindow.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"
#include "UMG_HotKeySlot.h"

UUMG_HotKeyWindow::UUMG_HotKeyWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{},
	m_ppHotKeySlot{}
{
	m_nMaxLength = (int32)EKEY::E_MAX - 1;
	m_ppHotKeySlot.Reserve(m_nMaxLength);
}

void UUMG_HotKeyWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	init();
}

void UUMG_HotKeyWindow::SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	// 매개변수로 들어온 Index는 들어오기 전 EKEY -> int32로 형변환이 이루어진 상태이다.

	// HotKeyComponent에서 각각에 해당하는 자리에서 들고있는 InterfaceSlot을 교환해줌.
	m_pMyController->HotKeyComponent->SwapHotKey((EKEY)nDragSlotIndex, (EKEY)nDropSlotIndex);

	m_ppHotKeySlot[nDragSlotIndex]->UpdateSlot();
	m_ppHotKeySlot[nDropSlotIndex]->UpdateSlot();
}

void UUMG_HotKeyWindow::ppHotKeySlotInit()
{
	m_ppHotKeySlot.Init(nullptr, m_nMaxLength);
	m_ppHotKeySlot[(int)EKEY::E_1] = Slot_1;
	m_ppHotKeySlot[(int)EKEY::E_2] = Slot_2;
	m_ppHotKeySlot[(int)EKEY::E_3] = Slot_3;
	m_ppHotKeySlot[(int)EKEY::E_4] = Slot_4;
	m_ppHotKeySlot[(int)EKEY::E_5] = Slot_5;
	m_ppHotKeySlot[(int)EKEY::E_6] = Slot_6;
}

void UUMG_HotKeyWindow::init()
{
	ppHotKeySlotInit();

	for (int i = 0; i < m_nMaxLength; i++)
	{
		m_ppHotKeySlot[i]->setWindowSlotInterface(this);
		m_ppHotKeySlot[i]->init();
	}
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

UUMG_HotKeySlot* UUMG_HotKeyWindow::getHotKeySlotFromIndex(int32 nHotKeySlotIndex)
{
	return nullptr;
}



