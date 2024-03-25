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

	// �ش� Ŭ������ �� ĳ����, NPC, ���͸� �� �����ϴ� �θ� Ŭ�����̴�.

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Effect")
	class UNiagaraSystem* m_FootStepNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Effect")
	class UNiagaraSystem* m_DefaultFootStepNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Sound")
	class USoundBase* m_DefaultLeftFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Sound")
	class USoundBase* m_LeftFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Sound")
	class USoundBase* m_RightFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Sound")
	class USoundBase* m_DefaultRightFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Effect")
	bool m_bBloodOn;
protected:
	ECharType m_eCharType;
public:	
	virtual bool IsAdventureMode();
	FUnitStat* getUnitStat();

	UFUNCTION(BlueprintCallable)
	UPARAM(ref)FUnitStat& getUnitRef() { return m_FStat; };

	UFUNCTION(BlueprintCallable)
	void setUnitRef(UPARAM(ref)FUnitStat& mFStat);

	ECharType getUnitType();
	virtual void DieEvent();
	virtual void DestroyChar();
	void Disappear();
	
	UFUNCTION(BlueprintImplementableEvent)
	void TakeDamageEvent(float fDamage);

	UFUNCTION(BlueprintCallable)
	void setUnitState(EUNIT_STATE eState);
	EUNIT_STATE getUnitState();

	virtual void MediateAttackRange(float fAddRangeAmount);

	// ======================== ���� ���� ===========================
	TMap<int32, class ABuff*> m_mapBuff; 

	void AddBuff(class ABuff* pBuff);
	void ReleaseBuff(ABuff* pBuff);
};
