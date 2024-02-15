// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "GrowingHero/UnitBase.h"
#include "BuffEffectBase.h"

#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/SkillComponent.h"
#include "GrowingHero/CombatComponent.h"
#include "Components/SphereComponent.h"

ABuff::ABuff() : 
	m_BuffName{},
	m_arBuffEffect{},
	m_arBuffEffectRef{},
	m_arBuffTargets{},
	m_fDetectRadius{},
	m_DetectTargetTagName{}
{
	BuffRange = CreateDefaultSubobject<USphereComponent>(TEXT("BuffRange"));
	BuffRange->SetupAttachment(RootComponent);
	BuffRange->SetSphereRadius(m_fDetectRadius);

	m_bIsDuplicate = true;

	m_arBuffTargets.Reserve(1000);
	m_arBuffEffectRef.Reserve(1000);
}

void ABuff::BeginPlay()
{
	Super::BeginPlay();
	m_arDetectFunc[(int)EDetectTargeType::E_Owner_Specify] = &ABuff::DetectOwner_Specify;
	m_arDetectFunc[(int)EDetectTargeType::E_Target_Specify] = &ABuff::DetectTarget_Specify;
	m_arDetectFunc[(int)EDetectTargeType::E_Owner_Range] = &ABuff::DetectRange;
	m_arDetectFunc[(int)EDetectTargeType::E_Target_Range] = &ABuff::DetectRange;
}

void ABuff::Disappear()
{
	// ��� Ÿ�ٿ� ���ؼ� ���� ���������ֱ� 
	for (AUnitBase* BuffTarget : m_arBuffTargets)
	{
		if (IsValid(BuffTarget))
		{
			ReleaseBuffFromTarget(BuffTarget); // ���� ȿ�� ����
			BuffTarget->ReleaseBuff(this); // Ÿ�����κ��� �� ���� �����ֱ�
		}
	}
	
	Super::Disappear();
}

void ABuff::ActivateSkill()
{
	findDetectTargetTagName(); // � Ÿ������ Detect�� ������ ���� 
	(this->*m_arDetectFunc[(int)m_eDetectType])(); // ������ Detect Type��� �Լ� ȣ��
	if (m_arBuffTargets.Num() <= 0)
	{
		m_pSkillOwner->getUnitStat()->m_MP += m_nConsumeMP;
		CallRelease();
		return;
	}
	SpanwSkillNiagara_Sound();
	LoadBuffEffect();

	Super::ActivateSkill();
	for (AUnitBase* BuffTarget : m_arBuffTargets)
	{
		if (IsValid(BuffTarget))
		{
			BuffTarget->AddBuff(this);
			for (auto BuffEffectRef : m_arBuffEffectRef)
			{
				BuffEffectRef->ApplyBuff(BuffTarget);
			}
		}
	}

	FTimerHandle DestroyTimer{};
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ABuff::CallRelease, m_fHoldingTime, false, (-1.0F));
}

void ABuff::init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum)
{
	Super::init(nSkillLV, nConsumeMP, nSkillNum);
	// ���ӽð� ����
	// ����ȿ���鿡�Ե� Lv �ֱ� 

	m_eTargetType = ECharType::E_Enemy;
	if (m_pSkillOwner->getUnitType() == ECharType::E_Enemy)
		m_eTargetType = ECharType::E_Hero;

	m_bIsActivated = true;
	ActivateSkill();
}


void ABuff::DetectOwner_Specify()
{
	m_arBuffTargets.Add(m_pSkillOwner);
}

void ABuff::DetectRange()
{
	TArray<AActor*> OverlappingActors{};
	m_arBuffTargets.Add(m_pSkillOwner);
	BuffRange->GetOverlappingActors(OverlappingActors, DetectFilter);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(m_DetectTargetTagName))
		{
			AUnitBase* AddUnit = Cast<AUnitBase>(Actor);
			m_arBuffTargets.Add(AddUnit);
		}
	}
}

void ABuff::DetectTarget_Specify()
{
	// Ŭ���� ���� ����ȿ�� ������� �ֱ� ����.
	AMyCharacterController* MyController = Cast<AMyCharacterController>(m_pSkillOwner->GetController());
	AUnitBase* pUnit = Cast<AUnitBase>(MyController->getClickedActor());
	if (IsValid(pUnit) && pUnit != nullptr && pUnit->ActorHasTag("Enemy"))
		m_arBuffTargets.Add(pUnit);
}


void ABuff::findDetectTargetTagName()
{
	m_DetectTargetTagName = "Hero";

	// Enemy�� ����? 
	// SkillOwner�� Enemy && EDetectTargetType�� E_Owner_Specify || E_Owner_Range
	// SkillOwner�� Hero && EDetectTargetType�� E_Target_Specify || E_Target_Range
	if((m_pSkillOwner->getUnitType() == ECharType::E_Enemy && (m_eDetectType == EDetectTargeType::E_Owner_Specify || m_eDetectType == EDetectTargeType::E_Owner_Range))
		||
		(m_pSkillOwner->getUnitType() == ECharType::E_Hero && (m_eDetectType == EDetectTargeType::E_Target_Specify || m_eDetectType == EDetectTargeType::E_Target_Range))
		)
	{
		m_DetectTargetTagName = "Enemy";
	}
}

void ABuff::LoadBuffEffect()
{
	// AUnitBase* Target = m_pOwnerClass.GetDefaultObject();
	// ������ ��� ȿ�� ����
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnParams;
	FRotator rotator;

	for (auto BuffEffectClass : m_arBuffEffect)
	{
		ABuffEffectBase* BuffEffect = world->SpawnActor<ABuffEffectBase>
			(BuffEffectClass.Get(), GetActorLocation(), rotator, spawnParams);
		m_arBuffEffectRef.Add(BuffEffect);
	}
}

void ABuff::ReleaseBuffFromTarget(AUnitBase* pUnit)
{
	for (int32 i = 0; i < m_arBuffTargets.Num(); i++)
	{
		if (m_arBuffTargets[i] == pUnit)
		{
			for (auto BuffEffectRef : m_arBuffEffectRef)
			{
				BuffEffectRef->ReleaseBuff(m_arBuffTargets[i]);
			}
			m_arBuffTargets[i] = nullptr;
		}
	}
}


