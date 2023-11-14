// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EquipmentWindow.h"
#include "UMG_EquipmentSlot.h"

UUMG_EquipmentWindow::UUMG_EquipmentWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{},
	m_arEquipmentSlot{}
{
	m_nMaxLength = (int)EEquipType::E_MAX;
	m_arEquipmentSlot.Reserve(m_nMaxLength);
	m_arEquipmentSlot.Init(nullptr, m_nMaxLength);
}

void UUMG_EquipmentWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_arEquipmentSlot[(int)EEquipType::E_HAT] = HatSlot;
	m_arEquipmentSlot[(int)EEquipType::E_ARMOR] = ArmorSlot;
	m_arEquipmentSlot[(int)EEquipType::E_WEAPON] = WeaponSlot;
	m_arEquipmentSlot[(int)EEquipType::E_SHOES] = ShoesSlot;
}

void UUMG_EquipmentWindow::UpdateEquipmentSlot(EEquipType eEquipType)
{
	m_arEquipmentSlot[(int)eEquipType]->UpdateSlot();
}
