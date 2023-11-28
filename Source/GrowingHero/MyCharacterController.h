// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StructureCollection.h"
#include "MyCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API AMyCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyCharacterController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* m_pClickedProp;

private:
	bool bClickMouse;
	float m_fInterfaceRagne;
	class AMyCharacter* m_pMyHero;

	float m_fMousePosX;
	float m_fMousePosY;
	float m_fTmpMousePosX;
	float m_fTmpMousePosY;

private:

	// ========= test ============
	class UPathFollowingComponent* InitNaviControl();
	//============================
	// private 멤버 함수
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// 키 입력 바이딩 함수
	void InputClickPressed();
	void InputClickReleased();
	void InventoryToggle();
	void EquipmentWindowToggle();
	void SkillWindowToggle();
	void StatWindowToggle();

	void HotKeyPressed(EKEY);
	// ===============================
	
	void SetNewDestination(const FVector DestLocation, AActor* DestActor, float DstDistance, bool IsEnemy);
	void MoveToMouseCursor();
public:
	UFUNCTION(BlueprintCallable)
	AActor* getClickedActor();
	void AttackAgain();
	FVector getMouseLocation();

public:
	// === inventory 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SystemComponent")
	class UInventoryComponent* InventorySystemComponent;

	// === HotKey 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SystemComponent")
	class UHotKeyComponent* HotKeyComponent;

	// === Equipment 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SystemComponent")
	class UEquipmentComponent* EquipmentComponent;

	// === Skill 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SystemComponent")
	class USkillComponent* SkillComponent;

	// === Stat 작업 ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SystemComponent")
	class UStatComponent* StatComponent;
};
