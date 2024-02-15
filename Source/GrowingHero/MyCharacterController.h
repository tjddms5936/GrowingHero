// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StructureCollection.h"
#include "MyCharacterController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDele_Dynamic);
DECLARE_DELEGATE(FDele_Single);

UCLASS()
class GROWINGHERO_API AMyCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyCharacterController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* m_pClickedProp;

	UPROPERTY(BlueprintReadWrite)
	class AMyCharacter* m_pMyHero;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic ED_ItemPickupPressed; // �������Ʈ���� ȣ���ϰ� �������� ���ε�

	FDele_Single ED_EnemyOutlineOff; // EnemyCharacter���� ȣ�� ���ε� ����.

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FDele_Dynamic ED_NPC_Conversation; // Ű ������ ȣ���ϰ� NPC �������Ʈ���� ���ε�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bClickMouse;
private:
	float m_fInterfaceRagne;

private:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;

public:
	// Ű �Է� ���̵� �Լ�
	UFUNCTION(BlueprintCallable)
	void InputClickPressed();

	UFUNCTION(BlueprintCallable)
	void InputClickReleased();

	UFUNCTION(BlueprintCallable)
	void InventoryToggle();

	UFUNCTION(BlueprintCallable)
	void EquipmentWindowToggle();

	UFUNCTION(BlueprintCallable)
	void SkillWindowToggle();

	UFUNCTION(BlueprintCallable)
	void StatWindowToggle();

	UFUNCTION(BlueprintCallable)
	void HotKeyPressed(EKEY eKey);

	UFUNCTION(BlueprintCallable)
	void NPC_ConversationKeyPressed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameMenuToggle();

	UFUNCTION(BlueprintCallable)
	void OpenedFrameTearDownPressed();
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void InitComponentSetting();
	void LoadUMGSetting();

public:
	UFUNCTION(BlueprintCallable)
	AActor* getClickedActor();
	void AttackAgain();
	FVector getMouseLocation();
	void setClickedActor(AActor* pActor);
	void CharLevelUp();
public:
	// === inventory �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventorySystemComponent;

	// === HotKey �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHotKeyComponent* HotKeyComponent;

	// === Equipment �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;

	// === Skill �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkillComponent* SkillComponent;

	// === Stat �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStatComponent* StatComponent;

	// === ī�޶� ���� �۾� ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraTransparencyComponent* CameraTransparencyComponent;


	// === UMG ===
	TQueue<class UUMG_Frame*> m_arOpenedFrame;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> InventoryFrameClass;
	UPROPERTY(BlueprintReadOnly, Category = "Customizing|UMG")
	class UUMG_InventoryFrame* InventoryFrame;
	UFUNCTION(BlueprintCallable)
	void InventoryFrameRefresh(EItemType eItemType);
	UFUNCTION(BlueprintCallable)
	void LoadInventoryHotKey(const TMap<int32, int32>& mapData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> EquipmentFrameClass;
	class UUMG_EquipmentFrame* EquipmentFrame;
	UFUNCTION(BlueprintCallable)
	void EquipmentFrameRefresh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> SkillFrameClass;
	UPROPERTY(BlueprintReadOnly, Category = "Customizing|UMG")
	class UUMG_SkillFrame* SkillFrame;
	UFUNCTION(BlueprintCallable)
	void SkillFrameRefresh();
	UFUNCTION(BlueprintCallable)
	void LoadSkillHotKey(const TMap<int32, int32>& mapData);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> StatFrameClass;
	UPROPERTY(BlueprintReadOnly)
	class UUMG_StatFrame* StatFrame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Customizing|UMG")
	TSubclassOf<class UUserWidget> GameHUDClass;
	UPROPERTY(BlueprintReadOnly)
	class UUMG_GameHUD* GameHUD;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameAndUIModeSetting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GameOnlyModeSetting();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UIOnlyModeSetting();
};
