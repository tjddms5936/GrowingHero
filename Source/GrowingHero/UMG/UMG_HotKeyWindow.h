// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface_Window_Slot.h"
#include "UMG_HotKeyWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_HotKeyWindow : public UUserWidget, public IInterface_Window_Slot
{
	GENERATED_BODY()
	
public:
	UUMG_HotKeyWindow(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void SwapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex) override;

public:
	int32 m_nMaxLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotArray")
	TArray<UUMG_HotKeySlot*> m_ppHotKeySlot;

	UFUNCTION(BlueprintCallable)
	class UUMG_HotKeySlot* getHotKeySlotFromIndex(int32 nHotKeySlotIndex);

private:
	void ppHotKeySlotInit();
	class AMyCharacterController* m_pMyController;

public:
	void init();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UUMG_HotKeySlot* Slot_6;
};


