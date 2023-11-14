// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_HotKeyWindow.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"

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
	ppHotKeySlotInit();

	for (int i = 0; i < m_nMaxLength; i++)
	{
		m_ppHotKeySlot[i]->setWindowSlotInterface(this);
	}
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
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
	m_ppHotKeySlot[(int)EKEY::E_Del] = Slot_Del;
	m_ppHotKeySlot[(int)EKEY::E_End] = Slot_End;
	m_ppHotKeySlot[(int)EKEY::E_Home] = Slot_Home;
	m_ppHotKeySlot[(int)EKEY::E_Ins] = Slot_Ins;
	m_ppHotKeySlot[(int)EKEY::E_PgDown] = Slot_PgDown;
	m_ppHotKeySlot[(int)EKEY::E_PgUp] = Slot_PgUp;
}

UUMG_HotKeySlot* UUMG_HotKeyWindow::getHotKeySlotFromIndex(int32 nHotKeySlotIndex)
{
	return nullptr;
}



