// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructureCollection.h"
#include "SkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 m_nSkillPoint;
	int32 m_nMaxLength;
	class UDataTable* SkillDB;
	class AUnitBase* m_pOwnerUnit;

public:
	TArray<FSkillData> m_arSkill;
	TArray<FName> m_arSkillMontageName;
	TMap<ESkillNum, ASkillBase*> m_mapCantDuplicatedSkill;

	UPROPERTY(BlueprintReadOnly)
	class AMyCharacterController* m_pMyController;
private:
	void init();
	void LoadSkillFrame();
	bool CheckMP(int32 nNeedMP);

	ASkillBase* SpawnSkill(ESkillNum eSkillNum);
public:
	// UMG
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UUserWidget> SkillFrameClass;

	class UUMG_SkillFrame* SkillFrame;


	void releaseSkill(ASkillBase* pReleaseSkill);
	void SKillLevelUp(ESkillNum eSkillNum);
	UFUNCTION(BlueprintCallable)
	int32 getSkillPoint();
	void UsingSkill(ESkillNum eSkillNum);
	void setOwnerUnit(AUnitBase* pOwnerUnit);
	void setOwnerController(AMyCharacterController* pController);
};


