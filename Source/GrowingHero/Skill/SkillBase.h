// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SkillBase.generated.h"

UCLASS()
class GROWINGHERO_API ASkillBase : public APawn
{
	GENERATED_BODY()

public:
	ASkillBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	void CallRelease();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* SpawnNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Customizing")
	class USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* DestroyNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Customizing")
	class USoundBase* DestroySound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AUnitBase* m_pSkillOwner;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	float m_fHoldingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	int32 m_nATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	int32 m_nSkillLv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Customizing")
	bool m_bIsDuplicate;

	int32 m_nConsumeMP;

public:
	virtual void Disappear();
	virtual void ActivateSkill();
	virtual void init(int32 nSkillLV, int32 nSkillMP);

	// 자식 클래스의 Disappear 시점에 Super::Disappear 부르기 전 즉, DestroyActor 직전에 작업하는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyEvent();


	void SpanwSkillNiagara_Sound();
	UFUNCTION(BlueprintCallable)
	void DestroySkillNiagara_Sound();

	void setSkillOwner(AUnitBase* pOwner);
};
