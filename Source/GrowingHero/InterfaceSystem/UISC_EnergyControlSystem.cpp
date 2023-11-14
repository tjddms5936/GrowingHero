// Fill out your copyright notice in the Description page of Project Settings.


#include "UISC_EnergyControlSystem.h"
#include "GrowingHero/InterfaceProp.h"
#include "GrowingHero/UnitBase.h"
void UUISC_EnergyControlSystem::AddHP()
{
	UE_LOG(LogTemp, Warning, TEXT("m_fAddHP : %f"), m_fAddHP);
	m_pInterfacedUnit->getUnitStat()->m_HP += m_fAddHP;
	if (m_pInterfacedUnit->getUnitStat()->m_HP >= m_pInterfacedUnit->getUnitStat()->m_MaxHP)
	{
		m_pInterfacedUnit->getUnitStat()->m_HP = m_pInterfacedUnit->getUnitStat()->m_MaxHP;
		UE_LOG(LogTemp, Warning, TEXT("Already Full HP %d"), m_pInterfacedUnit->getUnitStat()->m_HP);
	}
	else if (m_pInterfacedUnit->getUnitStat()->m_HP <= 0)
	{
		m_pInterfacedUnit->getUnitStat()->m_HP = 0;
		UE_LOG(LogTemp, Warning, TEXT("Player HP : %d - Player Die" ), m_pInterfacedUnit->getUnitStat()->m_HP);
	}
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, FString::Printf(TEXT("Player HP : %d"), m_pInterfacedUnit->getUnitStat()->m_HP));
}

void UUISC_EnergyControlSystem::ActiveSystem()
{
	if (!ensure(m_pInterfacedUnit != nullptr))
	{
		return;
	}

	AddHP();
}

void UUISC_EnergyControlSystem::setInitSystem(float fInitValue)
{
	setAddHP(fInitValue);
}

void UUISC_EnergyControlSystem::setAddHP(float fAddHP)
{
	m_fAddHP = fAddHP;
}
