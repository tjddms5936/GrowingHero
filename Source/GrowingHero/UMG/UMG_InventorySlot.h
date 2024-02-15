// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InterfaceWithHotKeySlot_Base.h"
//#include "GrowingHero/StructureCollection.h"
#include "GrowingHero/EnumCollection.h"
#include "UMG_InventorySlot.generated.h"

/**
 * 
 */

UCLASS()
class GROWINGHERO_API UUMG_InventorySlot : public UInterfaceWithHotKeySlot_Base
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClickBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SlotInfo")
	class UDataTable* ItemDB;

public:
	UUMG_InventorySlot(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

	// ================ ����Ű�� Interface �� �Լ��� ======================
	virtual void Activate() override;
	virtual void init() override;
	// ====================================================================
protected:
	class AMyCharacter* m_pMyHero;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	EItemType m_eItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nSlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	bool m_bEmpty;

	class IInterface_Window_Slot* m_WindowSlotInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotInfo")
	UTexture2D* m_pDefaultThumbnail;

public:
	UFUNCTION(BlueprintCallable)
	virtual void clearSlot();

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(FName ItemRowName);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem();

	UFUNCTION(BlueprintCallable)
	virtual void DropItem();

	UFUNCTION(BlueprintCallable)
	virtual void SwapSlot(int32 nDragSlotIndex);

	UFUNCTION(BlueprintCallable)
	virtual	void UpdataSlot();

	

	void setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToolTipToggle();
};
