// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_EquipmentWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_EquipmentWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_EquipmentWindow(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
private:
	int32 m_nMaxLength;
	
public:
	TArray<class UUMG_EquipmentSlot*> m_arEquipmentSlot;

public:
	void UpdateEquipmentSlot();
	void init();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* HatSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* ArmorSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* WeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_EquipmentSlot* ShoesSlot;
};
