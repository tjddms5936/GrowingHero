// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_SkillWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_SkillWindow(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!


private:
	int32 m_nMaxLength;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotArray")
	TArray<class UUMG_SkillSlot*> m_ppSlot;

private:

public:
	void init();

	UFUNCTION(BlueprintCallable)
	void UpdateSkillSlot(ESkillNum eSkillNum);

	void RefreshWindow();

	TMap<int32, int32> getEnteredHotKeyInfo();
	void LoadSkillHotKey(const TMap<int32, int32>& mapData);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUMG_SkillSlot* Slot_Skill6;
};
