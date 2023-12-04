// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "UMG/UMG_InventoryWindow.h"
#include "UMG//UMG_InventoryFrame.h"
#include "UMG/UMG_Inven_ConsumptionSlot.h"
#include "UMG/UMG_Inven_EquipmentSlot.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() :
	m_nMaxInventoryLength{},
	m_mapInventory_Consumption{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// default
	m_nMaxInventoryLength = 32;


	m_arAddItem[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::AddItem_Consumption;
	m_arAddItem[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::AddItem_Equipment;

	m_arSwapSlot[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::SwapSlot_Consumption;
	m_arSwapSlot[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::SwapSlot_Equipment;

	m_arClearItem[(int)EItemType::E_CONSUMPTION] = &UInventoryComponent::ClearItem_Consumption;
	m_arClearItem[(int)EItemType::E_EQUIPMENT] = &UInventoryComponent::ClearItem_Equipment;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	LoadInventoryFrame();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::getMaxInventoryLength()
{
	return m_nMaxInventoryLength;
}

bool UInventoryComponent::AddItem(EItemType eItemType, FName ItemRowName)
{
	return (this->*m_arAddItem[(int)eItemType])(ItemRowName);
}

void UInventoryComponent::ClearItem(EItemType eItemType, FName ItemName, int32 SlotIndex)
{
	(this->*m_arClearItem[(int)eItemType])(ItemName, SlotIndex);
}

void UInventoryComponent::swapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	(this->*m_arSwapSlot[(int)eItemType])(nDragSlotIndex, nDropSlotIndex);
}

void UInventoryComponent::LoadInventoryFrame()
{
	if (!ensure(InventoryFrameClass != nullptr))
		return;

	InventoryFrame = CreateWidget<UUMG_InventoryFrame>(GetWorld(), InventoryFrameClass, FName(InventoryFrameClass->GetName()));

}

bool UInventoryComponent::AddItem_Consumption(FName ItemRowName)
{
	FItemSlotIndex* pSlotIndex = m_mapInventory_Consumption.Find(ItemRowName);
	if (pSlotIndex == nullptr)
	{
		int nEmptySlotIndex{};
		if (!InventoryFrame->InventoryWindowConsumption->getEmptySlotIndex(nEmptySlotIndex))
		{
			return false;
		}

		FItemSlotIndex Struct_ItemSlotIndex{};
		Struct_ItemSlotIndex.arItemSlotIndex.Add(nEmptySlotIndex);
		m_mapInventory_Consumption.Add(ItemRowName, Struct_ItemSlotIndex);

		InventoryFrame->InventoryWindowConsumption->itemSetting(nEmptySlotIndex, ItemRowName);
		return true;
	}

	int32 nCurItemCount{};
	int32 nMaxItemCount{};
	UUMG_Inven_ConsumptionSlot* ConsumptionSlot = Cast<UUMG_Inven_ConsumptionSlot>(InventoryFrame->InventoryWindowConsumption->getSlotFromIndex(pSlotIndex->arItemSlotIndex.Last()));
	nCurItemCount = ConsumptionSlot->m_nItemCount;
	nMaxItemCount = ConsumptionSlot->m_nMaxItemCount;

	if (nCurItemCount < nMaxItemCount)
	{
		InventoryFrame->InventoryWindowConsumption->m_ppSlot[pSlotIndex->arItemSlotIndex.Last()]->m_nItemCount++;
	}
	else
	{
		int nEmptySlotIndex{};
		if (!InventoryFrame->InventoryWindowConsumption->getEmptySlotIndex(nEmptySlotIndex))
		{
			return false;
		}

		pSlotIndex->arItemSlotIndex.Add(nEmptySlotIndex);
		InventoryFrame->InventoryWindowConsumption->itemSetting(nEmptySlotIndex, ItemRowName);
	}
	return true;
}

bool UInventoryComponent::AddItem_Equipment(FName ItemRowName)
{
	FItemSlotIndex* pSlotIndex = m_mapInventory_Equipment.Find(ItemRowName);

	int nEmptySlotIndex{};
	if (!InventoryFrame->InventoryWindowEquipment->getEmptySlotIndex(nEmptySlotIndex))
		return false;

	if (pSlotIndex == nullptr)
	{
		FItemSlotIndex Struct_ItemSlotIndex{};
		Struct_ItemSlotIndex.arItemSlotIndex.Add(nEmptySlotIndex);
		m_mapInventory_Equipment.Add(ItemRowName, Struct_ItemSlotIndex);
	}
	else
		pSlotIndex->arItemSlotIndex.Add(nEmptySlotIndex);

	InventoryFrame->InventoryWindowEquipment->itemSetting(nEmptySlotIndex, ItemRowName);

	return true;
}

void UInventoryComponent::SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_ConsumptionSlot* DragSlot = Cast<UUMG_Inven_ConsumptionSlot>(InventoryFrame->InventoryWindowConsumption->m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_ConsumptionSlot* DropSlot = Cast<UUMG_Inven_ConsumptionSlot>(InventoryFrame->InventoryWindowConsumption->m_ppSlot[nDropSlotIndex]);
	FName DragItemName = DragSlot->m_FItem.ItemName;
	int32 nTmpSlotIndex = nDragSlotIndex;

	FItemSlotIndex* arSlotIndexTmp = m_mapInventory_Consumption.Find(DragItemName);
	if (arSlotIndexTmp == nullptr)
		return;

	int32 nOriginIndex = arSlotIndexTmp->arItemSlotIndex.Find(nDragSlotIndex);
	if (nOriginIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("nDragSlotIndex INDEX_NONE"));
		return;
	}

	arSlotIndexTmp->arItemSlotIndex[nOriginIndex] = nDropSlotIndex;

	FName DropItemName{};
	if (!(DropSlot->m_bEmpty))
	{
		DropItemName = DropSlot->m_FItem.ItemName;
		arSlotIndexTmp = m_mapInventory_Consumption.Find(DropItemName);
		if (!ensure(arSlotIndexTmp != nullptr))
			return;
		int32 nAnotherIndex = arSlotIndexTmp->arItemSlotIndex.Find(nDragSlotIndex);
		if (nAnotherIndex == INDEX_NONE)
			return;

		arSlotIndexTmp->arItemSlotIndex[nAnotherIndex] = nTmpSlotIndex;
	}
}

void UInventoryComponent::SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex)
{
	UUMG_Inven_EquipmentSlot* DragSlot = Cast<UUMG_Inven_EquipmentSlot>(InventoryFrame->InventoryWindowEquipment->m_ppSlot[nDragSlotIndex]);
	UUMG_Inven_EquipmentSlot* DropSlot = Cast<UUMG_Inven_EquipmentSlot>(InventoryFrame->InventoryWindowEquipment->m_ppSlot[nDropSlotIndex]);
	FName DragItemName = DragSlot->m_FEuqipItem.ItemName;
	int32 nTmpSlotIndex = nDragSlotIndex;

	FItemSlotIndex* arSlotIndexTmp = m_mapInventory_Equipment.Find(DragItemName);
	if (arSlotIndexTmp == nullptr)
		return;
	int32 nOriginIndex = arSlotIndexTmp->arItemSlotIndex.Find(nDragSlotIndex);
	if (nOriginIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("nDragSlotIndex INDEX_NONE"));
		return;
	}

	arSlotIndexTmp->arItemSlotIndex[nOriginIndex] = nDropSlotIndex;

	FName DropItemName{};
	if (!(DropSlot->m_bEmpty))
	{
		DropItemName = DropSlot->m_FEuqipItem.ItemName;
		arSlotIndexTmp = m_mapInventory_Equipment.Find(DropItemName);
		if (!ensure(arSlotIndexTmp != nullptr))
			return;
		int32 nAnotherIndex = arSlotIndexTmp->arItemSlotIndex.Find(nDragSlotIndex);
		if (nAnotherIndex == INDEX_NONE)
			return;

		arSlotIndexTmp->arItemSlotIndex[nAnotherIndex] = nTmpSlotIndex;
	}
}

void UInventoryComponent::ClearItem_Consumption(FName ItemName, int32 SlotIndex)
{
	FItemSlotIndex* mapTmp = m_mapInventory_Consumption.Find(ItemName);
	if (mapTmp->arItemSlotIndex.Find(SlotIndex))
	{
		mapTmp->arItemSlotIndex.Remove(SlotIndex);
	}
}

void UInventoryComponent::ClearItem_Equipment(FName ItemName, int32 SlotIndex)
{
	FItemSlotIndex* mapTmp = m_mapInventory_Equipment.Find(ItemName);
	if (mapTmp->arItemSlotIndex.Find(SlotIndex))
	{
		mapTmp->arItemSlotIndex.Remove(SlotIndex);
	}
}

