// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Frame.h"
#include "UMG_InventoryFrame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_InventoryFrame : public UUMG_Frame
{
	GENERATED_BODY()

public:
	UUMG_InventoryFrame(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

private:
	// bool m_bOpenFrame;
	TArray<class UUMG_InventoryWindow*> m_arWindow;

public:
	virtual void Setup() override;
	virtual void TearDown() override;
	virtual void init() override;

	UFUNCTION(BlueprintCallable)
	void ToggleWindow(UUMG_InventoryWindow* pOpenWindow);

	// �Լ������� �迭�� �κ��丮 â ���� �и��ؼ� ������Ʈ ����
	void UpdateInventoryWindow(enum class EItemType eItemType);
	void(UUMG_InventoryFrame::* m_arUpdateWindow[(int)EItemType::E_MAX])();

	// Key : �Һ������Slot Index����.  Value : HotKey Index ����. (int32)EKey �̴�.
	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> getEnteredHotKeyInfo();
	void LoadInventoryHotKey(const TMap<int32, int32>& mapData);

	UFUNCTION(BlueprintImplementableEvent)
	void SetupBtnColor();
private:
	void UpdateWindow_Consumption();
	void UpdateWindow_Equipment();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowConsumption;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_InventoryWindow* InventoryWindowEtc;

};
