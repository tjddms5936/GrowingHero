// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_StatWindow.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_StatWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUMG_StatWindow(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMyCharacter* m_pMyHero;

public:
	void init();
};
