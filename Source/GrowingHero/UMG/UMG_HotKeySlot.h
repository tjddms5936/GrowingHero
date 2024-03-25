// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_HotKeySlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_HotKeySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_HotKeySlot(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // BeginPlay
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HotKey Info")
	FText m_HotKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HotKey Info")
	EKEY m_eKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	bool m_bEmpty;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	int32 m_nQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	UTexture2D* m_pThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HotKey Info")
	UTexture2D* m_DefaultThumbnail;
	
	// �κ��丮 Slot�̳� ��ų Slot�� ��ϵ� ���̴�. ���������ʹ� UPROPERTY�� �ȵȴ�.
	class UInterfaceWithHotKeySlot_Base** m_ppInterfaceSlot;
	
	class IInterface_Window_Slot* m_WindowSlotInterface;

private:
	class AMyCharacterController* m_pMyController;

public:
	void init();
	void setWindowSlotInterface(IInterface_Window_Slot* WindowSlot_Interface);

	UFUNCTION(BlueprintCallable)
	void Activate();
	
	UFUNCTION(BlueprintCallable)
	void clearSlot();

	UFUNCTION(BlueprintCallable)
	void SwapSlot(EKEY eDragSlotKey);
	
	UFUNCTION()
	void UpdateSlot();

	UFUNCTION(BlueprintCallable)
	void UpdateSlotOnlyForBlueprint();

	UFUNCTION(BlueprintCallable)
	void ClearAlreadyEntered(UInterfaceWithHotKeySlot_Base* pInterfaceSlot);

	UFUNCTION(BlueprintCallable)
	UInterfaceWithHotKeySlot_Base* getInterfaceSlot();

	UFUNCTION(BlueprintCallable)
	void clearBeforeSettingSlot();

	UFUNCTION(BlueprintImplementableEvent)
	void CoolTimeStart();
};
