// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_InventoryWindow.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/InventoryComponent.h"
#include "GrowingHero/HotKeyComponent.h"

#include "UMG_Inven_ConsumptionSlot.h"
#include "UMG_Inven_EquipmentSlot.h"

UUMG_InventoryWindow::UUMG_InventoryWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{}
{
	m_nMaxLength = 35;
	m_ppSlot.Reserve(m_nMaxLength);

	m_arSwapSlot[(int)EItemType::E_CONSUMPTION] = &UUMG_InventoryWindow::SwapSlot_Consumption;
	m_arSwapSlot[(int)EItemType::E_EQUIPMENT] = &UUMG_InventoryWindow::SwapSlot_Equipment;
}

void UUMG_InventoryWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ppSlotInit();

	for (int i = 0; i < m_nMaxLength; i++)
	{
		m_ppSlot[i]->setWindowSlotInterface(this);
	}

	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

bool UUMG_InventoryWindow::getEmptySlotIndex(int32& nResult)
{
	for (int i = 0; i < m_nMaxLength; i++)
	{
		if (IsValid(m_ppSlot[i]) && m_ppSlot[i]->m_bEmpty)
		{
			nResult = i;
			return true;
		}
	}
	return false;
}

UUMG_InventorySlot* UUMG_InventoryWindow::getSlotFromIndex(int32 nSlotIndex)
{
	if(nSlotIndex >= m_nMaxLength && nSlotIndex < 0)
		return nullptr;
	return m_ppSlot[nSlotIndex];
}

void UUMG_InventoryWindow::ppSlotInit()
{
	m_ppSlot.Init(nullptr, m_nMaxLength);
	m_ppSlot[0] = Slot_Index0;
	m_ppSlot[1] = Slot_Index1;
	m_ppSlot[2] = Slot_Index2;
	m_ppSlot[3] = Slot_Index3;
	m_ppSlot[4] = Slot_Index4;

	m_ppSlot[5] = Slot_Index5;
	m_ppSlot[6] = Slot_Index6;
	m_ppSlot[7] = Slot_Index7;
	m_ppSlot[8] = Slot_Index8;
	m_ppSlot[9] = Slot_Index9;

	m_ppSlot[10] = Slot_Index10;
	m_ppSlot[11] = Slot_Index11;
	m_ppSlot[12] = Slot_Index12;
	m_ppSlot[13] = Slot_Index13;
	m_ppSlot[14] = Slot_Index14;

	m_ppSlot[15] = Slot_Index15;
	m_ppSlot[16] = Slot_Index16;
	m_ppSlot[17] = Slot_Index17;
	m_ppSlot[18] = Slot_Index18;
	m_ppSlot[19] = Slot_Index19;

	m_ppSlot[20] = Slot_Index20;
	m_ppSlot[21] = Slot_Index21;
	m_ppSlot[22] = Slot_Index22;
	m_ppSlot[23] = Slot_Index23;
	m_ppSlot[24] = Slot_Index24;

	m_ppSlot[25] = Slot_Index25;
	m_ppSlot[26] = Slot_Index26;
	m_ppSlot[27] = Slot_Index27;
	m_ppSlot[28] = Slot_Index28;
	m_ppSlot[29] = Slot_Index29;

	m_ppSlot[30] = Slot_Index30;
	m_ppSlot[31] = Slot_Index31;
	m_ppSlot[32] = Slot_Index32;
	m_ppSlot[33] = Slot_Index33;
	m_ppSlot[m_nMaxLength - 1] = Slot_Index34;
}

void UUMG_InventoryWindow::SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_ConsumptionSlot* pDragSlot = Cast<UUMG_Inven_ConsumptionSlot>(m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_ConsumptionSlot* pDropSlot = Cast<UUMG_Inven_ConsumptionSlot>(m_ppSlot[nDropSlotIndex]);
	// ========= HotKey에 알려주기 =========
	EKEY eDragSlotKey = pDragSlot->m_eMyHotKey;
	EKEY eDropSlotKey = pDropSlot->m_eMyHotKey;
	pDragSlot->setMyHotKey(eDropSlotKey);
	pDropSlot->setMyHotKey(eDragSlotKey);
	pDragSlot->enterHotKey();
	pDropSlot->enterHotKey();

	// ========= 인벤창 Swap 작업 =========
	// 컨트롤러가 가지고 있는 인벤토리 컴포넌트에 접근해서 Swap 작업을 진행해준다.
	m_pMyController->InventorySystemComponent->swapSlot(m_ppSlot[nDragSlotIndex]->m_eItemType, nDragSlotIndex, nDropSlotIndex);

	// 일단 Drag한 Slot의 정보를 가져와야 함.
	m_FTmpInventoryItem.CopyStructure(pDragSlot->m_FItem);

	int32 nTmpCurItemCount = pDragSlot->m_nItemCount;
	bool bTmpEmpty = pDragSlot->m_bEmpty;

	// Drop된 Slot은 Drag된 Slot의 정보를 그대로 갖고 와야 함

	if (pDropSlot->m_bEmpty)
	{
		// 만약 바꿀려는 Slot이 비어있다면? 
		pDragSlot->clearSlot();
	}
	else
	{
		pDragSlot->UpdataSlot(pDropSlot->m_FItem, pDropSlot->m_nItemCount, false);
	}

	pDropSlot->UpdataSlot(m_FTmpInventoryItem, nTmpCurItemCount, bTmpEmpty);
}

void UUMG_InventoryWindow::SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_EquipmentSlot* pDragSlot = Cast<UUMG_Inven_EquipmentSlot>(m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_EquipmentSlot* pDropSlot = Cast<UUMG_Inven_EquipmentSlot>(m_ppSlot[nDropSlotIndex]);

	// 컨트롤러가 가지고 있는 인벤토리 컴포넌트에 접근해서 Swap 작업을 진행해준다.
	m_pMyController->InventorySystemComponent->swapSlot(m_ppSlot[nDragSlotIndex]->m_eItemType, nDragSlotIndex, nDropSlotIndex);

	// 일단 Drag한 Slot의 정보를 가져와야 함.
	m_FTmpEquipmentItem.CopyStructure(pDragSlot->m_FEuqipItem);
	bool bTmpEmpty = pDragSlot->m_bEmpty;

	// Drop된 Slot은 Drag된 Slot의 정보를 그대로 갖고 와야 함
	if (pDropSlot->m_bEmpty)
	{
		// 만약 바꿀려는 Slot이 비어있다면? 
		pDragSlot->clearSlot();
	}
	else
	{
		pDragSlot->UpdataSlot(pDropSlot->m_FEuqipItem, false);
	}

	pDropSlot->UpdataSlot(m_FTmpEquipmentItem, bTmpEmpty);

}

void UUMG_InventoryWindow::SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	(this->*m_arSwapSlot[(int)eItemType])(nDragSlotIndex, nDropSlotIndex);
}
