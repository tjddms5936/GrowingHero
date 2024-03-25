// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "Skill/SkillBase.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
USkillComponent::USkillComponent() :
	m_nMaxLength{},
	m_bCanSkillActive{},
	m_bCanCoolDown{},
	m_nSkillPoint{},
	m_arSkill{},
	m_arSkillMontageName{},
	m_mapCantDuplicatedSkill{}
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void USkillComponent::initSkill()
{
	// m_arSkill �ʱ�ȭ
	for (int32 SkillNum = (int32)ESkillNum::E_Skill_1; SkillNum < (int32)ESkillNum::E_MAX; SkillNum++)
	{
		FString RowName = FString::FromInt(SkillNum);
		if ((SkillDB->FindRow<FSkillData>(FName(*RowName) , RowName) != nullptr))
		{
			m_arSkill[SkillNum] = *(SkillDB->FindRow<FSkillData>(FName(*RowName), RowName));
			if (!m_arSkill[SkillNum].CanDuplicated)
				m_mapCantDuplicatedSkill.Add((ESkillNum)SkillNum, nullptr);
		}
	}

	// ======================================= ������ �� ��ų �ϳ� �ְ� ���� =============================
	m_arSkill[0].SKill_CurLev++;
	m_arSkill[0].SKill_CoolTime += m_arSkill[0].LvUpChangeSkill_CoolTime;
	m_arSkill[0].Skill_MP += m_arSkill[0].LvUpChangeSkill_MP;
	// =================================================================================================

	// m_arSkillMontageName �ʱ�ȭ
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_1] = FName("Projectile");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_2] = FName("Summon");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_3] = FName("Summon");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_4] = FName("Projectile");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_5] = FName("Buff");
	m_arSkillMontageName[(int32)ESkillNum::E_Skill_6] = FName("Buff");
}

bool USkillComponent::ConsumeMP(int32 nNeedMP)
{
	if (m_pOwnerUnit->getUnitStat()->m_MP >= nNeedMP)
	{
		m_pOwnerUnit->getUnitStat()->m_MP -= nNeedMP;
		UE_LOG(LogTemp, Warning, TEXT("MP Consume"));
		return true;
	}

	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Red, FString::Printf(TEXT("Need more MP")));
	return false;
}

void USkillComponent::init()
{
	m_nSkillPoint = 4;

	m_nMaxLength = (int32)ESkillNum::E_MAX;

	m_arSkill.Reserve(m_nMaxLength);
	m_arSkill.Init(FSkillData(), m_nMaxLength);

	m_arSkillMontageName.Reserve(m_nMaxLength);
	m_arSkillMontageName.Init(FName(), m_nMaxLength);

	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pOwnerUnit = Cast<AMyCharacter>(m_pMyController->GetPawn());

	// ��ų ��� ���� �ʱ�ȭ 
	m_bCanSkillActive.Reserve(m_nMaxLength);
	m_bCanSkillActive.Init(true, m_nMaxLength);
	m_bCanCoolDown.Reserve(m_nMaxLength);
	m_bCanCoolDown.Init(false, m_nMaxLength);
	
	initSkill();
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
	if (m_arSkill[(int32)eSkillNum].SkillType == ESkillType::E_SummonsFix)
	{
		location = m_pMyController->getMouseLocation();
	}
	ASkillBase* SpawnedSkill = world->SpawnActor<ASkillBase>(m_arSkill[(int)eSkillNum].SpawnSkill.Get(), location, rotator, spawnParams);
	SpawnedSkill->ED_ActivateSuccessful.BindUFunction(this, FName("SkillAnimPlay"));
	SpawnedSkill->setSkillOwner(m_pOwnerUnit);
	SpawnedSkill->init(m_arSkill[(int32)eSkillNum].SKill_CurLev, m_arSkill[(int32)eSkillNum].Skill_MP, (int32)eSkillNum);

	return SpawnedSkill;
}

void USkillComponent::resetSkillCoolTime(int32 nSkillNum)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Reset"));
	m_bCanSkillActive[nSkillNum] = true;
	m_bCanCoolDown[nSkillNum] = false;
}

void USkillComponent::SkillAnimPlay(int32 nSkillNum)
{
	if (m_pOwnerUnit == nullptr)
		return;

	if (m_pOwnerUnit->getUnitState() == EUNIT_STATE::E_Dead ||
		m_pOwnerUnit->getUnitState() == EUNIT_STATE::E_UnderAttack)
		return;

	m_pOwnerUnit->setUnitState(EUNIT_STATE::E_Attack);
	UAnimInstance* AnimIns = m_pOwnerUnit->GetMesh()->GetAnimInstance();
	if (AnimIns && SkillMontage)
	{
		AnimIns->Montage_Play(SkillMontage, 1.f); // 0.5���ϸ� ���� ���ǵ� 1/2��
		AnimIns->Montage_JumpToSection(m_arSkillMontageName[nSkillNum], SkillMontage);
	}
	m_bCanCoolDown[nSkillNum] = true;

	// ��ų ��Ÿ�� ����
	if (m_arSkill[nSkillNum].SKill_CoolTime <= 0)
		return;

	m_bCanSkillActive[nSkillNum] = false;
	FTimerHandle ResetTimer{};
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this, nSkillNum] {
		if (IsValid(this))
			this->resetSkillCoolTime(nSkillNum);
		else
		{
			//this died before callback triggered
		}
		}, m_arSkill[nSkillNum].SKill_CoolTime, false);
}

void USkillComponent::SKillLevelUp(ESkillNum eSkillNum)
{
	if (m_arSkill.IsValidIndex((int32)eSkillNum) && 
		m_arSkill[(int32)eSkillNum].SKill_CurLev < m_arSkill[(int32)eSkillNum].Skill_MaxLev)
	{
		m_arSkill[(int32)eSkillNum].SKill_CurLev++;
		m_arSkill[(int32)eSkillNum].SKill_CoolTime += m_arSkill[(int32)eSkillNum].LvUpChangeSkill_CoolTime;
		m_arSkill[(int32)eSkillNum].Skill_MP += m_arSkill[(int32)eSkillNum].LvUpChangeSkill_MP;
		m_nSkillPoint--;
		m_pMyController->SkillFrameRefresh();
	}
}

int32 USkillComponent::getSkillPoint()
{
	return m_nSkillPoint;
}

void USkillComponent::UsingSkill(ESkillNum eSkillNum)
{
	if (m_arSkill.IsValidIndex((int32)eSkillNum) &&
		m_arSkill[(int32)eSkillNum].SpawnSkill != nullptr && 
		ConsumeMP(m_arSkill[(int32)eSkillNum].Skill_MP))
	{
		// ��Ÿ���� �ִ� ��ų�̰� ���� ��Ÿ���� �����ִٸ� ��ų �ߵ� ����s
		if (!m_bCanSkillActive[(int32)eSkillNum])
		{
			ConsumeMP(m_arSkill[(int32)eSkillNum].Skill_MP * -1);
			return;
		}

		// �ߺ� ������ �Ұ����� ��ų�̶�� (Summons Skill) �����Ȱ� ó�� �ϰ� �����ؾ� �Ѵ�.
		if (!m_arSkill[(int32)eSkillNum].CanDuplicated && *(m_mapCantDuplicatedSkill.Find(eSkillNum)) != nullptr)
		{
			releaseSkill(*(m_mapCantDuplicatedSkill.Find(eSkillNum)));
		}

		// ��ų ����
		ASkillBase* pSpawnedSkill = SpawnSkill(eSkillNum);
		
		// ������ ��ų. '�ߺ� �ȵǴ� ��ų�ε� ������ ��ų' ������� TMap�� ���� Key : eSkillNum  Value : �����Ǿ��ִ� ��ų
		if (m_mapCantDuplicatedSkill.Contains(eSkillNum))
			*(m_mapCantDuplicatedSkill.Find(eSkillNum)) = pSpawnedSkill;
	}
}

void USkillComponent::AddSkillPoint(int32 nPoint)
{
	m_nSkillPoint += nPoint;
}

void USkillComponent::SkillAnimPlayEnd()
{
	m_pOwnerUnit->setUnitState(EUNIT_STATE::E_Idle);
}

bool USkillComponent::IsSkillCoolTimeOK(ESkillNum eSkillNum)
{
	return m_bCanCoolDown[(int32)eSkillNum];
}

bool USkillComponent::IsSkillCanActivat(ESkillNum eSkillNum)
{
	return m_bCanSkillActive[(int32)eSkillNum];
}

