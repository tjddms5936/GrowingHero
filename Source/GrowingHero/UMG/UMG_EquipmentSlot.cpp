// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EquipmentSlot.h"
#include "UMG_Inven_EquipmentSlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/EquipmentComponent.h"
#include "GrowingHero/InventoryComponent.h"

UUMG_EquipmentSlot::UUMG_EquipmentSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_eEquipType{},
	m_bEmpty{},
	m_FEquipment{}
{
	m_bEmpty = true;
}

void UUMG_EquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
}

void UUMG_EquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_EquipmentSlot::UnEquipItem()
{
	// 이 함수가 호출되는 경우는 2가지이다. 두 가지 경우 모두 인벤 장비창으로 돌려주어야 한다.
	// 1. 인벤 장비창에서 아이템 장착했는데, 이미 해당부위에 장착되어있는 아이템이 있을 경우
	// 2. 장비창에서 더블클릭해서 아이템 해제하는 경우.
	// 
	// 우선은 인벤에 아이템 등록해준다.
	m_pMyController->InventorySystemComponent->AddItem(EItemType::E_EQUIPMENT, m_FEquipment.ItemName);

	// EquipmentComponent에 등록된 것을 초기화해준다.
	m_pMyController->EquipmentComponent->ReleaseEquipment(m_eEquipType);
	
	UpdateSlot();
}

void UUMG_EquipmentSlot::EquipItem(UUMG_Inven_EquipmentSlot* pDragSlot)
{
	// 인벤장비창에서 장비창으로 Drag & Drop 하는 경우 호출 된다.
	if (pDragSlot->m_FEuqipItem.ItemEquipType != m_eEquipType)
		return;

	pDragSlot->Activate();
	UpdateSlot();
}

void UUMG_EquipmentSlot::DropItem()
{
	// 장비창에서 다이렉트로 필드로 Drop하는 경우에 호출 된다.
	if (!ensure(m_FEquipment.FieldItemClass != nullptr))
		return;
	
	// 필드로 버려준다.
	FTransform SpawnTransform{};
	SpawnTransform = m_pMyHero->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
	UWorld* world = GetWorld();
	world->SpawnActor<AInterfaceProp_Inventory>(m_FEquipment.FieldItemClass.Get(), SpawnTransform);

	// EquipmentComponent에 등록된 것을 초기화해준다.
	m_pMyController->EquipmentComponent->ReleaseEquipment(m_eEquipType);

	UpdateSlot();
}

void UUMG_EquipmentSlot::UpdateSlot()
{
	m_FEquipment.CopyStructure((m_pMyController->EquipmentComponent->m_arStrEquipItem[(int)m_eEquipType]));
	m_pThumbnail = m_FEquipment.Thumbnail;
	m_bEmpty = true;
	if (m_pThumbnail != nullptr)
	{
		m_bEmpty = false;
	}
}
