// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillWindow.h"

UUMG_SkillWindow::UUMG_SkillWindow(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nMaxLength{},
	m_ppSlot{}
{
	m_nMaxLength = (int)ESkillNum::E_MAX;
	m_ppSlot.Reserve(m_nMaxLength);
	m_ppSlot.Init(nullptr, m_nMaxLength);
}

void UUMG_SkillWindow::NativeOnInitialized()
{
	init();
}

void UUMG_SkillWindow::NativeConstruct()
{

}

void UUMG_SkillWindow::init()
{
	m_ppSlot[(int)ESkillNum::E_Skill_1] = Slot_Skill1;
	m_ppSlot[(int)ESkillNum::E_Skill_2] = Slot_Skill2;
	m_ppSlot[(int)ESkillNum::E_Skill_3] = Slot_Skill3;
	m_ppSlot[(int)ESkillNum::E_Skill_4] = Slot_Skill4;
	m_ppSlot[(int)ESkillNum::E_Skill_5] = Slot_Skill5;
	m_ppSlot[(int)ESkillNum::E_Skill_6] = Slot_Skill6;


}

void UUMG_SkillWindow::UpdateSkillSlot(ESkillNum eSkillNum)
{
	if (m_ppSlot.IsValidIndex((int)eSkillNum))
	{
		m_ppSlot[(int)eSkillNum]->UpdateSlot();
	}
}
