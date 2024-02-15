// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceWithHotKeySlot_Base.h"
#include "GrowingHero/StructureCollection.h"
#include "UMG_SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillSlot : public UInterfaceWithHotKeySlot_Base
{
	GENERATED_BODY()

public:
	UUMG_SkillSlot(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

	// ================== ����Ű�� Interface �Ǵ� �Լ� =====================
	virtual void Activate() override;
	virtual UTexture2D* getThumbnail() override;
	virtual void init() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* LevelUpBtn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	ESkillNum m_eSkillNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FSkillData m_FSkillData;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateSlot();
	
	UFUNCTION(BlueprintCallable)
	bool CanLvUp();

	UFUNCTION(BlueprintCallable)
	void LvUp();

	UFUNCTION(BlueprintCallable)
	void UseSkill();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateThumbnail();

	UFUNCTION(BlueprintImplementableEvent)
	void CoolTimeStart();
};
