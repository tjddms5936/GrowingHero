// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_HotKeySlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"

UUMG_HotKeySlot::UUMG_HotKeySlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_eKey{},
	m_bEmpty{}
{
	m_bEmpty = true;
}

void UUMG_HotKeySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_ppInterfaceSlot = &(m_pMyController->HotKeyComponent->m_arHotKey[(int)m_eKey]);
}

void UUMG_HotKeySlot::NativeConstruct()
{
	Super::NativeConstruct();

	// HotKeyComponent이 들고있는 배열에 접근해서 경로 지정해준다.  
	// m_ppInterfaceSlot = &(m_pMyController->HotKeyComponent->m_arHotKey[(int)m_eKey]);
}


void UUMG_HotKeySlot::Activate()
{
	if ((*m_ppInterfaceSlot) == nullptr)
		return;
	(*m_ppInterfaceSlot)->Activate();
	UpdateSlot();
	
}

void UUMG_HotKeySlot::setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface)
{
	m_WindowSlotInterface = WindowSlot_Interface;
}

void UUMG_HotKeySlot::SwapSlot(EKEY eDragSlotKey)
{
	if (!ensure(m_WindowSlotInterface != nullptr) && m_bEmpty == true)
		return;

	m_WindowSlotInterface->SwapSlot(EItemType::E_NONE, (int)eDragSlotKey, (int)m_eKey);
}

void UUMG_HotKeySlot::clearSlot()
{
	(*m_ppInterfaceSlot) = nullptr;
	m_bEmpty = true;
	m_nQuantity = 0;
	m_pThumbnail = nullptr;
}

void UUMG_HotKeySlot::UpdateSlot()
{
	if ((*m_ppInterfaceSlot) == nullptr)
	{
		clearSlot();
		return;
	}
	m_bEmpty = false;
	m_pThumbnail = (*m_ppInterfaceSlot)->m_pThumbnail;

	if ((*m_ppInterfaceSlot)->m_bIsCountable)
	{
		m_nQuantity = (*m_ppInterfaceSlot)->m_nItemCount;
	}
}

void UUMG_HotKeySlot::ClearAlreadyEntered(UInterfaceWithHotKeySlot_Base* pInterfaceSlot)
{
	// 똑같은 아이템이 단축키에 세팅되어 있는 경우 정리해주는 함수
	for (auto v : m_pMyController->HotKeyComponent->m_arHotKey)
	{
		if (v == pInterfaceSlot)
		{
			v->clearHotKey();
			return;
		}
	}
}

UInterfaceWithHotKeySlot_Base* UUMG_HotKeySlot::getInterfaceSlot()
{
	return *m_ppInterfaceSlot;
}

void UUMG_HotKeySlot::clearBeforeSettingSlot()
{
	// 해당 Slot에 이미 등록된 단축키가 있다면 정리해주기
	if (getInterfaceSlot() == nullptr)
		return;

	getInterfaceSlot()->clearHotKey();
}




