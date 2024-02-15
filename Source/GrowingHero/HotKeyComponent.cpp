// Fill out your copyright notice in the Description page of Project Settings.


#include "HotKeyComponent.h"
#include "UMG/InterfaceWithHotKeySlot_Base.h"

// Sets default values for this component's properties
UHotKeyComponent::UHotKeyComponent() :
	m_nMaxLength{},
	m_arHotKey{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_nMaxLength = (int)EKEY::E_MAX;
	m_arHotKey.Reserve(m_nMaxLength);
	m_arHotKey.Init(nullptr, m_nMaxLength);
}


// Called when the game starts
void UHotKeyComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHotKeyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UHotKeyComponent::setHotKey(EKEY eKey, UInterfaceWithHotKeySlot_Base* pInterfaceSlot)
{
	m_arHotKey[(int)eKey] = pInterfaceSlot;
}

void UHotKeyComponent::SwapHotKey(EKEY eDragSlotKey, EKEY eDropSlotKey)
{
	// ����Ű ������ Swap�� ���� �Ҹ���. 
	// DragSlot�� �׻� �����Ѵ�. �׷��� Drag�� �ȴ�. ������ DropSlot�� �������� �������� �ִ�. 

	// ��� ����̴� DragSlot�� ������ ����صξ�� �Ѵ�. 
	UInterfaceWithHotKeySlot_Base* pBackUpDragSlot = m_arHotKey[(int)eDragSlotKey];
	UInterfaceWithHotKeySlot_Base* pBackUpDropSlot = m_arHotKey[(int)eDropSlotKey];

	// case1 : DropSlot�� ������� �� 
	if (m_arHotKey[(int)eDropSlotKey] == nullptr)
	{
		pBackUpDragSlot->clearHotKey();
		pBackUpDragSlot->setMyHotKey(eDropSlotKey);
		pBackUpDragSlot->enterHotKey();
	}

	else
	{
		pBackUpDragSlot->clearHotKey();
		pBackUpDragSlot->setMyHotKey(eDropSlotKey);

		pBackUpDropSlot->clearHotKey();
		pBackUpDropSlot->setMyHotKey(eDragSlotKey);
		
		pBackUpDragSlot->enterHotKey();
		pBackUpDropSlot->enterHotKey();
	}
}

void UHotKeyComponent::ActivateHotKey(EKEY eActivateKey)
{
	if (m_arHotKey[(int)eActivateKey] != nullptr)
	{
		m_arHotKey[(int)eActivateKey]->Activate();
	}
}




