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
	FDele_Dynamic ED_ItemPickupPressed;

	FDele_Single ED_EnemyOutlineOff; 
private:
	bool bClickMouse;
	float m_fInterfaceRagne;

	float m_fMousePosX;
	float m_fMousePosY;
	float m_fTmpMousePosX;
	float m_fTmpMousePosY;

private:

	// ========= test ============
	// class UPathFollowingComponent* InitNaviControl();
	//============================
	// private 멤버 함수
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// 키 입력 바이딩 함수
public:
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
	// ===============================
	
	/*void SetNewDestination(const FVector DestLocation, AActor* DestActor, float DstDistance, bool IsEnemy);
	void MoveToMouseCursor();*/
private:
	void MoveForward(float Value);
	void MoveRight(float Value);


public:
	UFUNCTION(BlueprintCallable)
	AActor* getClickedActor();
	void AttackAgain();
	FVector getMouseLocation();
	void setClickedActor(AActor* pActor);

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
