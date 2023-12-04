// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface_Window_Slot.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_InventoryWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_InventoryWindow : public UUserWidget, public IInterface_Window_Slot
{
	GENERATED_BODY()
public:
	UUMG_InventoryWindow(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다

	virtual void SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotArray")
	TArray <class  UUMG_InventorySlot*> m_ppSlot;

	// m_ppSlot 배열에서 가장 처음부터 검색하면서 빈 슬롯을 찾아서 반환
	bool getEmptySlotIndex(int32& nResult);

	// 이 함수가 불린다는 것은 무조건 아이템 주웠을 때이다.
	UFUNCTION(BlueprintImplementableEvent)
	void itemSetting(int32 nSlotIndex, FName ItemRowName);

	UFUNCTION(BlueprintCallable)
	UUMG_InventorySlot* getSlotFromIndex(int32 nSlotIndex);

public:
	void(UUMG_InventoryWindow::* m_arSwapSlot[(int)EItemType::E_MAX])(int32 nDragSlotIndex, int32 nDropSlotIndex);

private:
	int32 m_nMaxLength;
	FInventoryItem m_FTmpInventoryItem;
	FInventoryEquipment m_FTmpEquipmentItem;
	class AMyCharacterController* m_pMyController;

private:
	void ppSlotInit();
	void SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index6;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index7;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index8;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index9;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index11;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index12;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index13;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index14;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index15;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index16;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index17;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index18;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index19;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index20;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index21;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index22;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index23;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index24;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index25;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index26;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index27;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index28;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index29;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index30;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index31;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index32;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index33;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_InventorySlot* Slot_Index34;

};

