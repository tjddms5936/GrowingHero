// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_EquipmentSlot.h"
#include "UMG_Inven_EquipmentSlot.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/EquipmentComponent.h"

UUMG_EquipmentSlot::UUMG_EquipmentSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_eEquipType{},
	m_bEmpty{},
	m_FEquipment{}
{

}

void UUMG_EquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_EquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_EquipmentSlot::init()
{
	m_bEmpty = true;
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
}

void UUMG_EquipmentSlot::UnEquipItem()
{
	// EquipmentComponent�� ��ϵ� ���� �ʱ�ȭ���ش�.
	m_pMyController->EquipmentComponent->ReleaseEquipment(m_eEquipType, false);
	
	UpdateSlot();
}

void UUMG_EquipmentSlot::EquipItem(UUMG_Inven_EquipmentSlot* pDragSlot)
{
	// �κ����â���� ���â���� Drag & Drop �ϴ� ��� ȣ�� �ȴ�.
	if (pDragSlot->m_FEuqipItem.ItemEquipType != m_eEquipType)
		return;

	pDragSlot->Activate();
	UpdateSlot();
}

void UUMG_EquipmentSlot::DropItem()
{
	// ���â���� ���̷�Ʈ�� �ʵ�� Drop�ϴ� ��쿡 ȣ�� �ȴ�.
	if (!ensure(m_FEquipment.FieldItemClass != nullptr))
		return;
	
	// �ʵ�� �����ش�.
	FTransform SpawnTransform{};
	SpawnTransform = m_pMyHero->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
	UWorld* world = GetWorld();
	world->SpawnActor<AInterfaceProp_Inventory>(m_FEquipment.FieldItemClass.Get(), SpawnTransform);

	// EquipmentComponent�� ��ϵ� ���� �ʱ�ȭ���ش�.
	m_pMyController->EquipmentComponent->ReleaseEquipment(m_eEquipType, true);
	

	UpdateSlot();
}

void UUMG_EquipmentSlot::UpdateSlot()
{
	m_FEquipment.CopyStructure((m_pMyController->EquipmentComponent->m_arStrEquipItem[(int)m_eEquipType]));
	m_pThumbnail = m_FEquipment.Thumbnail;
	m_bEmpty = true;

	if (m_pThumbnail != nullptr)
		m_bEmpty = false;
	TTUpdate();

	if(m_pThumbnail == nullptr)
		m_pThumbnail = m_pDefaultThumbnail;
}
