// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG_InventorySlot.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_Inven_EquipmentSlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_Inven_EquipmentSlot : public UUMG_InventorySlot
{
	GENERATED_BODY()
	
public:
	UUMG_Inven_EquipmentSlot(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	FInventoryEquipment m_FEuqipItem;

public:
	// ================ ����Ű�� Interface �� �Լ��� ======================
	virtual void Activate() override;

public:
	virtual void clearSlot() override;

	virtual void AddItem(FName ItemRowName) override;

	virtual void UseItem() override;

	virtual void DropItem() override;

	virtual void SwapSlot(int32 nDragSlotIndex) override;

	// =================================== new =============================
	virtual void UpdataSlot() override;

	virtual void init() override;
};
