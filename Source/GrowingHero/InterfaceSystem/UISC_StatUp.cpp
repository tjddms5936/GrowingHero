// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_StatUp.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"

UUISC_StatUp::UUISC_StatUp()
{

}

void UUISC_StatUp::ActiveSystem()
{
	AMyCharacter* MyHero = Cast<AMyCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (MyHero == nullptr)
		return;

	MyHero->getUnitStat()->m_MaxHP += m_nMaxHP;
	MyHero->getUnitStat()->m_MaxMP += m_nMaxMP;
	MyHero->getUnitStat()->CurEXP += m_nCurEXP;
	MyHero->getUnitStat()->ATK += m_nATK;
	MyHero->getUnitStat()->DEF += m_nDEF;
	MyHero->getUnitStat()->STR += m_nSTR;
	MyHero->getUnitStat()->DEX += m_nDEX;
	MyHero->getUnitStat()->INT += m_nINT;
	MyHero->getUnitStat()->LUK += m_nLUK;
	MyHero->getUnitStat()->AttackRange += m_fAttackRange;
	MyHero->getUnitStat()->AttackSpeed += m_fAttackSpeed;

	MyHero->MediateAttackRange(m_fAttackRange);
}

void UUISC_StatUp::setInitSystem(int32 nMaxHP, int32 nMaxMP, int32 nCurEXP, int32 nATK, int32 nDEF, int32 nSTR, int32 nDEX, int32 nINT, int32 nLUK, float fAttackRange, float fAttackSpeed)
{
	m_nMaxHP = nMaxHP;
	m_nMaxMP = nMaxMP;
	m_nCurEXP = nCurEXP;
	m_nATK = nATK;
	m_nDEF = nDEF;
	m_nSTR = nSTR;
	m_nDEX = nDEX;
	m_nINT = nINT;
	m_nLUK = nLUK;
	m_fAttackRange = fAttackRange;
	m_fAttackSpeed = fAttackSpeed;
}
