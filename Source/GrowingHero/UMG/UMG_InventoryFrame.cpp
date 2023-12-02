// Fill out your copyright notice in the Description page of Project Settings.


#include "GrowingHero/UMG/UMG_InventoryFrame.h"
#include "GrowingHero/UMG/UMG_InventoryWindow.h"

UUMG_InventoryFrame::UUMG_InventoryFrame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_arWindow{}
{
	m_arWindow.Reserve(3);
	m_arWindow.Init(nullptr, 3);
}

void UUMG_InventoryFrame::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_arWindow[0] = InventoryWindowConsumption;
	m_arWindow[1] = InventoryWindowEquipment;
	m_arWindow[2] = InventoryWindowEtc;

}

void UUMG_InventoryFrame::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_InventoryFrame::Setup()
{
	Super::Setup();
	ToggleWindow(InventoryWindowConsumption);
}

void UUMG_InventoryFrame::TearDown()
{
	Super::TearDown();
}


void UUMG_InventoryFrame::ToggleWindow(UUMG_InventoryWindow* pOpenWindow)
{
	for (auto ar : m_arWindow)
	{
		if (ar == pOpenWindow)
		{
			ar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
