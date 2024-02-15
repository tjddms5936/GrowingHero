// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG_Frame.h"
#include "UMG_SkillFrame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillFrame : public UUMG_Frame
{
	GENERATED_BODY()

public:
	UUMG_SkillFrame(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override;
	virtual void init() override;
	virtual void Setup() override;
	virtual void TearDown() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_SkillWindow* SkillWindow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 m_nCurSkillPoint;

	void UpdateSkillWindow();

	// Key : SkillSlot Index����.  Value : HotKey Index ����. (int32)EKey �̴�.
	UFUNCTION(BlueprintCallable)
	TMap<int32, int32> getEnteredHotKeyInfo(); 

	void LoadSkillHotKey(const TMap<int32, int32>& mapData);
};
