// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_GameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_GameHUD(const FObjectInitializer& ObjectInitializer); // 생성자
	virtual void NativeOnInitialized() override; // 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void init();
public:
	UFUNCTION(BlueprintCallable)
	class UUMG_InventoryWindow* getInventoryWindow(EItemType eItemType);

private:
	UUMG_InventoryWindow*(UUMG_GameHUD::* m_arGetInventoryWindow[(int)EItemType::E_MAX])();
	UUMG_InventoryWindow* getInventoryWindowConsumption();
	UUMG_InventoryWindow* getInventoryWindowEquipment();
	UUMG_InventoryWindow* getInventoryWindowEtc();

private:
	class AMyCharacterController* m_pMyController;

};
