// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG_Frame.h"
#include "UMG_SkillWindow.h"
#include "UMG_SkillFrame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_SkillFrame : public UUMG_Frame
{
	GENERATED_BODY()

public:
	UUMG_SkillFrame(const FObjectInitializer& ObjectInitializer); // »ý¼ºÀÚ
	virtual void NativeOnInitialized() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUMG_SkillWindow* SkillWindow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMyCharacterController* MyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 m_nCurSkillPoint;
};
