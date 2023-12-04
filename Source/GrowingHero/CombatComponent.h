// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumCollection.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AUnitBase* m_pTarget;
	ECharType m_eTargetType;
	FTimerHandle AttackTimer;
	FTimerHandle DeathTimer;

	AUnitBase* Owner;

	void RotateToTarget(float DeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf <class UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimInstance* AnimInstance;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TakeDamage(float Damage);
	UFUNCTION(BlueprintCallable)
	void TakeDamageEnd();

	UFUNCTION(BlueprintCallable)
	void ApplyDamage();

	void setInit(AUnitBase* pTarget, ECharType eTargetType);
	void clearTarget();

	void Die();
	UFUNCTION(BlueprintCallable)
	void DieEnd();

	UFUNCTION(BlueprintCallable)
	void ReleaseCollision();

	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	AUnitBase* getTarget();
	void setTargetClear();
};
