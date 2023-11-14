// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceWithHotKeySlot_Base.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/HotKeyComponent.h"

UInterfaceWithHotKeySlot_Base::UInterfaceWithHotKeySlot_Base(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nItemCount{},
	m_bIsCountable{},
	m_eMyHotKey{}
{
	// 스킬의 경우 아이템 갯수 개념이 없다. boolean으로 구분하자.
	m_bIsCountable = true;
	m_eMyHotKey = EKEY::E_Default;
}

void UInterfaceWithHotKeySlot_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UInterfaceWithHotKeySlot_Base::NativeConstruct()
{

}

void UInterfaceWithHotKeySlot_Base::Activate()
{
}

void UInterfaceWithHotKeySlot_Base::setMyHotKey(EKEY eKey)
{
	m_eMyHotKey = eKey;
}

void UInterfaceWithHotKeySlot_Base::enterHotKey()
{
	if (m_eMyHotKey != EKEY::E_Default)
	{
		m_pMyController->HotKeyComponent->setHotKey(m_eMyHotKey, this);
	}
}

void UInterfaceWithHotKeySlot_Base::clearHotKey()
{
	if (m_eMyHotKey != EKEY::E_Default)
	{
		m_pMyController->HotKeyComponent->setHotKey(m_eMyHotKey, nullptr);
		m_eMyHotKey = EKEY::E_Default;
	}
}

