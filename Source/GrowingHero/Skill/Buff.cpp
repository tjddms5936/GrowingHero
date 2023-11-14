// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff.h"
#include "GrowingHero/UnitBase.h"
#include "BuffEffectBase.h"

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
	// 모든 타겟에 대해서 버프 해제시켜주기 
	for (AUnitBase* BuffTarget : m_arBuffTargets)
	{
		if (IsValid(BuffTarget))
		{
			ReleaseBuffFromTarget(BuffTarget); // 버프 효과 해제
			BuffTarget->ReleaseBuff(this); // 타겟으로부터 이 버프 지워주기
		}
	}
	
	Super::Disappear();
}

void ABuff::ActivateSkill()
{
	findDetectTargetTagName(); // 어떤 타입으로 Detect할 것인지 결정 
	(this->*m_arDetectFunc[(int)m_eDetectType])(); // 결정한 Detect Type대로 함수 호출
	if (m_arBuffTargets.Num() <= 0)
	{
		CallRelease();
		return;
	}

	m_pSkillOwner->getUnitStat()->m_MP -= m_nConsumeMP; // 마나소모
	SpanwSkillNiagara_Sound();
	LoadBuffEffect();

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

void ABuff::init(int32 nSkillLV, int32 nSkillMP)
{
	Super::init(nSkillLV, nSkillMP);
	// 지속시간 조정
	// 버프효과들에게도 Lv 주기 

	m_eTargetType = ECharType::E_Enemy;
	if (m_pSkillOwner->getUnitType() == ECharType::E_Enemy)
		m_eTargetType = ECharType::E_Hero;


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
	if(IsValid(m_pSkillOwner->CombatComponent->getTarget()))
		m_arBuffTargets.Add(m_pSkillOwner->CombatComponent->getTarget());
}


void ABuff::findDetectTargetTagName()
{
	m_DetectTargetTagName = "Hero";

	// Enemy인 경우는? 
	// SkillOwner가 Enemy && EDetectTargetType이 E_Owner_Specify || E_Owner_Range
	// SkillOwner가 Hero && EDetectTargetType이 E_Target_Specify || E_Target_Range
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
	// 아이템 사용 효과 시전
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


