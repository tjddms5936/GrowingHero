// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "MyCharacter.generated.h"

/**
 * 
 */

UCLASS()
class GROWINGHERO_API AMyCharacter : public AUnitBase
{
	GENERATED_BODY()

public:
    AMyCharacter();
    
private:
    virtual void BeginPlay() override;
    virtual void initStat(float MaxHP) override;
    UFUNCTION()
    virtual void CombatRangeOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    virtual void CombatRangeOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    class AMyCharacterController* m_pMyController;
    class AEnemyCharacter* m_pEnemy;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USpringArmComponent* CameraSpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* CombatRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|MyHero")
    class UNiagaraSystem* LevelUpEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit|MyHero")
    class USoundBase* LevelUpSound;
public:
    void AttackEnemy(class AEnemyCharacter* Enemy);
    void GainEXT(int32 nEXP);
    void LevelUp();

    virtual void MediateAttackRange(float fAddRangeAmount) override;
};
