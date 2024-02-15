// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InterfaceWithHotKeySlot_Base.generated.h"

/**
 *
 */
DECLARE_DELEGATE(FDele_Single);

UCLASS()
class GROWINGHERO_API UInterfaceWithHotKeySlot_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	class AMyCharacterController* m_pMyController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	bool m_bIsCountable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	UTexture2D* m_pThumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	EKEY m_eMyHotKey;

	FDele_Single ED_CoolTimeStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SlotInfo")
	int32 m_nCoolTime;
public:
	UInterfaceWithHotKeySlot_Base(const FObjectInitializer& ObjectInitializer); // ������
	virtual void NativeOnInitialized() override; // ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�
	virtual void NativeConstruct() override; // AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!

	UFUNCTION(BlueprintCallable)
	virtual void Activate();
	virtual void init();

	void clearHotKey();

	UFUNCTION(BlueprintCallable)
	void setMyHotKey(EKEY eKey);

	UFUNCTION(BlueprintCallable)
	void enterHotKey();

	virtual UTexture2D* getThumbnail();
};



