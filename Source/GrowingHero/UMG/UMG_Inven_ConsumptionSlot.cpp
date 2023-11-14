// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_Inven_ConsumptionSlot.h"
#include "Engine/DataTable.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/InventoryComponent.h"

UUMG_Inven_ConsumptionSlot::UUMG_Inven_ConsumptionSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	m_eItemType = EItemType::E_CONSUMPTION;
	ItemDB = LoadObject<UDataTable>(nullptr, TEXT("'/Game/DataBase/InventoryItem_Consumption_DB.InventoryItem_Consumption_DB'"));
}

void UUMG_Inven_ConsumptionSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUMG_Inven_ConsumptionSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMG_Inven_ConsumptionSlot::Activate()
{
	UseItem();
}

void UUMG_Inven_ConsumptionSlot::clearSlot()
{
	m_pMyController->InventorySystemComponent->ClearItem(m_eItemType, m_FItem.ItemName, m_nSlotIndex);

	m_nItemCount = 0;
	m_nMaxItemCount = 0;
	m_bEmpty = true;
	m_pThumbnail = m_pDefaultThumbnail;

	m_FItem.ClearStructure();
}


void UUMG_Inven_ConsumptionSlot::AddItem(FName ItemRowName)
{
	if ((ItemDB->FindRow<FInventoryItem>(ItemRowName, ItemRowName.ToString()) == nullptr))
	{
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("No Item in DB")));
		return;
	}
	m_FItem = *(ItemDB->FindRow<FInventoryItem>(ItemRowName, ItemRowName.ToString()));

	m_nMaxItemCount = m_FItem.nMaxCount;
	m_bEmpty = false;
	m_nItemCount = 1;
	m_pThumbnail = m_FItem.Thumbnail;

	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("Add Item!")));
}

void UUMG_Inven_ConsumptionSlot::UseItem()
{
	if (!ensure(m_FItem.ItemClass != nullptr))
		return;

	// 아이템 사용 효과 시전
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;
	AInterfaceProp_Inventory* SpawnedItem = world->SpawnActor<AInterfaceProp_Inventory>(m_FItem.ItemClass.Get(), m_pMyHero->GetActorLocation(), rotator, spawnParams);
	if (SpawnedItem)
	{
		SpawnedItem->setInterfacedUnit(m_pMyHero);
		SpawnedItem->ActivateSystem();
		SpawnedItem->DestroyActor();
	}

	// UMG 변화 작업
	m_nItemCount--;
	if (m_nItemCount > 0)
		return;


	clearHotKey();
	clearSlot();
}

void UUMG_Inven_ConsumptionSlot::DropItem()
{
	if (!ensure(m_FItem.ItemClass != nullptr))
		return;

	FTransform SpawnTransform{};
	SpawnTransform = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetMesh()->GetSocketTransform(FName("DropItemPoint"));

	UWorld* world = GetWorld();
	world->SpawnActor<AInterfaceProp_Inventory>(m_FItem.ItemClass.Get(), SpawnTransform);

	m_nItemCount--;
	if (m_nItemCount > 0)
		return;


	clearHotKey();
	clearSlot();

}

void UUMG_Inven_ConsumptionSlot::SwapSlot(int32 nDragSlotIndex)
{
	if (!ensure(m_WindowSlotInterface != nullptr))
		return;
	m_WindowSlotInterface->SwapSlot(m_eItemType ,nDragSlotIndex, m_nSlotIndex);
}

void UUMG_Inven_ConsumptionSlot::UpdataSlot(FInventoryItem FItem, int32 nCurItemCount, bool bEmpty)
{
	m_FItem.CopyStructure(FItem);

	m_nItemCount = nCurItemCount;
	m_bEmpty = bEmpty;
	m_nMaxItemCount = m_FItem.nMaxCount;
	m_pThumbnail = m_FItem.Thumbnail;
}