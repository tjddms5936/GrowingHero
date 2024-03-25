// Fill out your copyright notice in the Description page of Project Settings.
#include "UnitBase.h"
#include "CombatComponent.h"
#include "TimerManager.h"
#include "Skill/Buff.h"
#include "StructureCollection.h"

// Sets default values
AUnitBase::AUnitBase() : 
	m_FStat{},
	m_eUnitState{},
	m_mapBuff{}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_eUnitState = EUNIT_STATE::E_Idle;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	GetMesh()->bReceivesDecals = false;
}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnitBase::initStat(float MaxHP)
{
}

void AUnitBase::MediateAttackRange(float fAddRangeAmount)
{
}

void AUnitBase::AddBuff(ABuff* pBuff)
{
	int nBuffSkill_ID{};
	nBuffSkill_ID = pBuff->m_nSkillID;
	ABuff** BuffRef = m_mapBuff.Find(nBuffSkill_ID);
	if (BuffRef == nullptr)
	{
		// ��ϵ� ���� ����. ���Ӱ� ��� 
		m_mapBuff.Add(nBuffSkill_ID, pBuff);
	}
	else
	{
		// �ʱ�ȭ ���־�� �Ѵ�. ���� �������� ���� ���� Ÿ�ٵ� �� ���� ������ �Ѵ�. 
		(*BuffRef)->ReleaseBuffFromTarget(this); // ���� ���� ȿ�� ����.
		ReleaseBuff(*BuffRef); // Map���� ���� ���� ����
		AddBuff(pBuff); // �Ȱ����� ���Ӱ� ���� ���� �ٽ� �߰��ϵ��� ȣ��
	}
}

void AUnitBase::ReleaseBuff(ABuff* pBuff)
{
	// m_arBuff���� pBuff �����ֱ� 
	int nBuffSkill_ID{};
	nBuffSkill_ID = pBuff->m_nSkillID;
	ABuff** BuffRef = m_mapBuff.Find(nBuffSkill_ID);
	if (BuffRef != nullptr)
	{
		// TMap���� �ش� ���� ������ �Ѵ�.  
		m_mapBuff.Remove(nBuffSkill_ID);
		*BuffRef = nullptr;
	}
}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AUnitBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (getUnitState() == EUNIT_STATE::E_Dead)
		return 0.f;
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (CombatComponent)
		CombatComponent->TakeDamage(Damage);
	return Damage;
}


bool AUnitBase::IsAdventureMode()
{
	return false;
}

FUnitStat* AUnitBase::getUnitStat()
{
	return &m_FStat;
}

void AUnitBase::setUnitRef(UPARAM(ref)FUnitStat& mFStat)
{
	m_FStat.ATK += mFStat.ATK;
	m_FStat.DEF += mFStat.DEF;
	m_FStat.STR += mFStat.STR;
	m_FStat.INT += mFStat.INT;
	m_FStat.DEX += mFStat.DEX;
	m_FStat.LUK += mFStat.LUK;
	m_FStat.AttackRange += mFStat.AttackRange;
	m_FStat.AttackSpeed += mFStat.AttackSpeed;
	m_FStat.Level += mFStat.Level;
	m_FStat.m_HP += mFStat.m_HP;
	m_FStat.m_MaxHP += mFStat.m_MaxHP;
	m_FStat.m_MP += mFStat.m_MP;
	m_FStat.m_MaxMP += mFStat.m_MaxMP;
}

ECharType AUnitBase::getUnitType()
{
	return m_eCharType;
}


void AUnitBase::DieEvent()
{
}

void AUnitBase::DestroyChar()
{
	SetActorEnableCollision(false);
}

void AUnitBase::Disappear()
{
	Destroy();
}

void AUnitBase::setUnitState(EUNIT_STATE eState)
{
	m_eUnitState = eState;
}

EUNIT_STATE AUnitBase::getUnitState()
{
	return m_eUnitState;
}
