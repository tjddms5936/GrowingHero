// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "UMG/UMG_SkillFrame.h"
#include "UMG/UMG_SkillWindow.h"
#include "Skill/SkillBase.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "Engine/DataTable.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent() :
	m_nSkillPoint{},
	m_nMaxLength{},
	m_arSkill{},
	m_arSkillMontageName{},
	m_mapCantDuplicatedSkill{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_nSkillPoint = 10;
	
	m_nMaxLength = (int)ESkillNum::E_MAX;
	
	m_arSkill.Reserve(m_nMaxLength);
	m_arSkill.Init(FSkillData(), m_nMaxLength);

	m_arSkillMontageName.Reserve(m_nMaxLength);
	m_arSkillMontageName.Init(FName(), m_nMaxLength);

	SkillDB = LoadObject<UDataTable>(nullptr, TEXT("'/Game/DataBase/SkillData_DB.SkillData_DB'"));
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pOwnerUnit = Cast<AMyCharacter>(m_pMyController->GetPawn());

	init();
	LoadSkillFrame();
	
}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void USkillComponent::init()
{
	// m_arSkill 초기화
	for (int SkillNum = (int)ESkillNum::E_Skill_1; SkillNum < (int)ESkillNum::E_MAX; SkillNum++)
	{
		FString RowName = FString::FromInt(SkillNum);
		if ((SkillDB->FindRow<FSkillData>(FName(*RowName) , RowName) != nullptr))
		{
			m_arSkill[SkillNum] = *(SkillDB->FindRow<FSkillData>(FName(*RowName), RowName));

			if (!m_arSkill[SkillNum].CanDuplicated)
				m_mapCantDuplicatedSkill.Add((ESkillNum)SkillNum, nullptr);

			UE_LOG(LogTemp, Warning, TEXT("Find Skill : %s"), *m_arSkill[SkillNum].Skill_Name.ToString());
		}
		else
			return;
	}

	// m_arSkillMontageName 초기화
	m_arSkillMontageName[(int)ESkillNum::E_Skill_1] = FName("EnergyBolt");
	m_arSkillMontageName[(int)ESkillNum::E_Skill_2] = FName("SpawnHero");
	m_arSkillMontageName[(int)ESkillNum::E_Skill_3] = FName("SpawnHydra");
	m_arSkillMontageName[(int)ESkillNum::E_Skill_4] = FName("Explosion");
	m_arSkillMontageName[(int)ESkillNum::E_Skill_5] = FName("StopEnemy");
}

void USkillComponent::LoadSkillFrame()
{
	if (!ensure(SkillFrameClass != nullptr))
		return;

	SkillFrame = CreateWidget<UUMG_SkillFrame>(GetWorld(), SkillFrameClass, FName(SkillFrameClass->GetName()));
	if (!SkillFrame)
		return;

	for (int SkillNum = (int)ESkillNum::E_Skill_1; SkillNum < (int)ESkillNum::E_MAX; SkillNum++)
	{
		SkillFrame->SkillWindow->UpdateSkillSlot(ESkillNum(SkillNum));
	}
	SkillFrame->m_nCurSkillPoint = m_nSkillPoint;
}

bool USkillComponent::CheckMP(int32 nNeedMP)
{
	if (m_pOwnerUnit->getUnitStat()->m_MP >= nNeedMP)
	{
		return true;
	}

	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, FString::Printf(TEXT("Need more MP")));
	return false;
}

void USkillComponent::releaseSkill(ASkillBase* pReleaseSkill)
{
	const ESkillNum* find = m_mapCantDuplicatedSkill.FindKey(pReleaseSkill);
	if (find != nullptr)
	{
		(*m_mapCantDuplicatedSkill.Find(*find))->Disappear();
		*(m_mapCantDuplicatedSkill.Find(*find)) = nullptr;
		return;
	}

	pReleaseSkill->Disappear();
}

ASkillBase* USkillComponent::SpawnSkill(ESkillNum eSkillNum)
{
	if (!IsValid(m_pOwnerUnit))
		return nullptr;
	UWorld* world = GetWorld();
	
	FActorSpawnParameters spawnParams{};
	FRotator rotator{};
	FVector location{};

	rotator = m_pOwnerUnit->GetActorRotation();
	
	location = m_pOwnerUnit->GetMesh()->GetSocketLocation("SpawnSkillPoint");
	if (m_arSkill[(int)eSkillNum].SkillType == ESkillType::E_SummonsFix)
	{
		location = m_pMyController->getMouseLocation();
	}
	ASkillBase* SpawnedSkill = world->SpawnActor<ASkillBase>(m_arSkill[(int)eSkillNum].SpawnSkill.Get(), location, rotator, spawnParams);
	SpawnedSkill->setSkillOwner(m_pOwnerUnit);
	SpawnedSkill->init(m_arSkill[(int)eSkillNum].SKill_CurLev, m_arSkill[(int)eSkillNum].Skill_MP);

	return SpawnedSkill;
}

void USkillComponent::SKillLevelUp(ESkillNum eSkillNum)
{
	if (m_nSkillPoint <= 0)
		return;

	if (m_arSkill.IsValidIndex((int)eSkillNum) && 
		m_arSkill[(int)eSkillNum].SKill_CurLev < m_arSkill[(int)eSkillNum].Skill_MaxLev)
	{
		m_arSkill[(int)eSkillNum].SKill_CurLev++;
		m_nSkillPoint--;
		SkillFrame->m_nCurSkillPoint--;
	}
}

int32 USkillComponent::getSkillPoint()
{
	return m_nSkillPoint;
}

void USkillComponent::UsingSkill(ESkillNum eSkillNum)
{
	if (m_arSkill.IsValidIndex((int)eSkillNum) &&
		m_arSkill[(int)eSkillNum].SpawnSkill != nullptr && 
		CheckMP(m_arSkill[(int)eSkillNum].Skill_MP))
	{
		// 쿨타임이 있는 스킬이고 아직 쿨타임이 남아있다면 스킬 발동 실패

		// 중복 스폰이 불가능한 스킬이라면 (Summons Skill) 스폰된거 처리 하고 스폰해야 한다.
		if (!m_arSkill[(int)eSkillNum].CanDuplicated && *(m_mapCantDuplicatedSkill.Find(eSkillNum)) != nullptr)
		{
			releaseSkill(*(m_mapCantDuplicatedSkill.Find(eSkillNum)));
		}

		// 마나 소모는 스폰된 스킬에서 처리해야 한다. 만약 스폰될 조건은 다 되었는데 스폰된 스킬에서 적이 없어서 바로 없어진다면?
		//m_pOwnerUnit->getUnitStat()->m_MP -= m_arSkill[(int)eSkillNum].Skill_MP;

		// 스킬 스폰
		ASkillBase* pSpawnedSkill = SpawnSkill(eSkillNum);


		// 스폰된 스킬. '중복 안되는 스킬인데 스폰된 스킬' 저장소인 TMap에 저장 Key : eSkillNum  Value : 스폰되어있는 스킬
		if (m_mapCantDuplicatedSkill.Contains(eSkillNum))
			*(m_mapCantDuplicatedSkill.Find(eSkillNum)) = pSpawnedSkill;
	}
}