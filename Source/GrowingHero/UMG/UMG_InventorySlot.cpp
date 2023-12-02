// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_InventorySlot.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"

#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"

UUMG_InventorySlot::UUMG_InventorySlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_eItemType{},
	m_nSlotIndex{},
	m_bEmpty{}
{
	
}

void UUMG_InventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetCharacter());
}

void UUMG_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_InventorySlot::Activate()
{
	
}



void UUMG_InventorySlot::clearSlot()
{
	
}


void UUMG_InventorySlot::AddItem(FName ItemRowName)
{
	
}

void UUMG_InventorySlot::UseItem()
{
	
}

void UUMG_InventorySlot::DropItem()
{
	
}

void UUMG_InventorySlot::SwapSlot(int32 nDragSlotIndex)
{
	
}


void UUMG_InventorySlot::setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface)
{
	// Slot에서는 인터페이스 포인터 등록할 수 있다.
	m_WindowSlotInterface = WindowSlot_Interface;
}

