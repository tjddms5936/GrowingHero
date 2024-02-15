// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG_Frame.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_Frame : public UUserWidget
{
	GENERATED_BODY()

public:
	UUMG_Frame(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

public:
	bool m_bOpenFrame;
	UPROPERTY(BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;
	class AMyCharacter* m_pMyHero;

public:
	virtual void Setup();

	UFUNCTION(BlueprintCallable)
	virtual void TearDown();

	UFUNCTION(BlueprintCallable)
	virtual void init();

	bool getToggleState();
	void ToggleFrame();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frame|Customizing")
	class USoundBase* SetupSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frame|Customizing")
	class USoundBase* TearDownSound;
};
