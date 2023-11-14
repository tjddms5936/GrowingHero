// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillSlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/SkillComponent.h"

UUMG_SkillSlot::UUMG_SkillSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void UUMG_SkillSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UUMG_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateSlot();
}

void UUMG_SkillSlot::Activate()
{
	UseSkill();
}

void UUMG_SkillSlot::UpdateSlot()
{
	m_FSkillData.CopyStructure(m_pMyController->SkillComponent->m_arSkill[(int)m_eSkillNum]);
	m_pThumbnail = m_FSkillData.Thumbnail;
}

bool UUMG_SkillSlot::CanLvUp()
{
	FName PreSkill = m_FSkillData.Pre_Skill_ID;
	if (PreSkill == FName("None"))
		return true;

	int32 index = FCString::Atof(*PreSkill.ToString());
	if (m_pMyController->SkillComponent->m_arSkill.IsValidIndex(index))
	{
		int32 PreeSkillCurLv = m_pMyController->SkillComponent->m_arSkill[index].SKill_CurLev;
		if (PreeSkillCurLv >= m_FSkillData.Pre_Skill_Lev && m_FSkillData.SKill_CurLev < m_FSkillData.Skill_MaxLev)
			return true;
	}

	return false;
}

void UUMG_SkillSlot::LvUp()
{
	m_pMyController->SkillComponent->SKillLevelUp(m_eSkillNum);
	UpdateSlot();
}

void UUMG_SkillSlot::UseSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("UseSkill"));
	m_pMyController->SkillComponent->UsingSkill(m_eSkillNum);
}
