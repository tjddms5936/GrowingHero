// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StructureCollection.h"
#include "UnitBase.generated.h"



UCLASS(Abstract)
class GROWINGHERO_API AUnitBase : public ACharacter
{
	GENERATED_BODY()

	// 해당 클래스는 내 캐릭터, NPC, 몬스터를 총 관리하는 부모 클래스이다.

public:
	// Sets default values for this character's properties
	AUnitBase();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void initStat(float MaxHP);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|UnitInfo")
	FUnitStat m_FStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit|UnitInfo")
	EUNIT_STATE m_eUnitState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit|Component")
	class UCombatComponent* CombatComponent;

protected:
	ECharType m_eCharType;
public:	
	FUnitStat* getUnitStat();

	UFUNCTION(BlueprintCallable)
	UPARAM(ref)FUnitStat& getUnitRef() { return m_FStat; };

	UFUNCTION(BlueprintCallable)
	void setUnitRef(UPARAM(ref)FUnitStat& mFStat);

	ECharType getUnitType();
	virtual void DieEvent();
	void DestroyChar();
	void Disappear();
	
	UFUNCTION(BlueprintImplementableEvent)
	void TakeDamageEvent(float fDamage);

	UFUNCTION(BlueprintCallable)
	void setUnitState(EUNIT_STATE eState);
	EUNIT_STATE getUnitState();

	virtual void MediateAttackRange(float fAddRangeAmount);

	// ======================== 버프 관련 ===========================
	TMap<int32, class ABuff*> m_mapBuff; 

	void AddBuff(class ABuff* pBuff);
	void ReleaseBuff(ABuff* pBuff);
};
