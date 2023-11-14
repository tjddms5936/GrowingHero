// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/EnemyCharacter.h"

AProjectile::AProjectile() :
	m_fProjectileSpeed{},
	m_arTargetInRange{},
	IsDestroy{},
	IsInitOK{}
{
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);

	DetectTargetRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectTargetRange"));
	DetectTargetRange->SetupAttachment(RootComponent);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;

	m_arTargetInRange.Reserve(1000);
	
	m_bIsDuplicate = true;
}



void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

}

void AProjectile::Disappear()
{
	DestroyEvent(); // 데미지 주고 이펙트 뿌리는 기능. 블루프린트에서 구현
	Super::Disappear();
}

void AProjectile::ActivateSkill()
{
	DetectTarget();
	if (m_pTarget)
	{
		m_pSkillOwner->getUnitStat()->m_MP -= m_nConsumeMP; // 마나소모
		SpanwSkillNiagara_Sound();
		LookAtTarget();
		setProjectileSpeed();
		setHomingTarget(m_pTarget);
	}
	// Target으로 발사. 만약 거리가 일정 범위 이하라면? 또는 이미 Collision 안에 Target이 있다면? 바로 데미지 주고 삭제
}

void AProjectile::init(int32 nSkillLV, int32 nSkillMP)
{
	Super::init(nSkillLV, nSkillMP);
	m_nATK *= nSkillLV;
	IsInitOK = true;

	ActivateSkill();

	// IsDestroy 의미 : Spawn 시점  <-> Init '시점 사이'에 바로 Target이 Overlap되는 경우 OverlapBegin함수에서 Destroy되는거 방지
	if (!ProjectileMovement->bIsHomingProjectile || IsDestroy)
	{
		Disappear();
		return;
	}

	FTimerHandle DestroyTimer{};
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AProjectile::CallRelease, m_fHoldingTime, false, (-1.0F));
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 적에 닿으면 데미지 입히고 삭제
	if (!OtherActor->ActorHasTag("Enemy"))
		return;

	if (m_pTarget && m_pTarget == Cast<AEnemyCharacter>(OtherActor))
	{
		DestroySkillNiagara_Sound();
		ApplyDmg();
		if (IsInitOK)
		{
			Disappear();
		}
		IsDestroy = true;
	}
}

void AProjectile::ApplyDmg()
{
	// 데미지 주는 기능
	TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	m_pTarget->TakeDamage(m_nATK, DamageEvent, GetController(), this);
}

AEnemyCharacter* AProjectile::getTarget()
{
	return m_pTarget;
}

void AProjectile::DetectTarget()
{
	// Target 세팅
	AMyCharacterController* HeroController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (HeroController == nullptr)
		return;

	if (HeroController->m_pClickedProp && HeroController->m_pClickedProp->ActorHasTag("Enemy"))
	{
		m_pTarget = Cast<AEnemyCharacter>(HeroController->m_pClickedProp);
	}

	if (m_pTarget)
		return;

	// 내 캐릭터가 Target을 클릭 안한 상황이다.
	// 시야 범위 내에서 가장 가까운 적을 찾는 알고리즘이 필요하다.
	FindTargetInRange();

}

void AProjectile::FindTargetInRange()
{
	TArray<AActor*> OverlappedActors{};

	DetectTargetRange->GetOverlappingActors(OverlappedActors, nullptr);

	for (AActor* actor : OverlappedActors)
	{
		if (actor->ActorHasTag("Enemy"))
		{
			AEnemyCharacter* pTmpEnemy = Cast<AEnemyCharacter>(actor);
			if (pTmpEnemy->getUnitState() != EUNIT_STATE::E_Dead)
			{
				m_arTargetInRange.Add(Cast<AEnemyCharacter>(actor));
			}
		}
	}

	if (m_arTargetInRange.Num() <= 0)
		return;

	FindClosestTarget();
}

void AProjectile::FindClosestTarget()
{
	float nMinDistance{};
	nMinDistance = 100000;
	for (auto Enemy : m_arTargetInRange)
	{
		if (GetDistanceTo(Enemy) < nMinDistance)
		{
			m_pTarget = Enemy;
		}
	}
}

void AProjectile::LookAtTarget()
{
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTarget->GetActorLocation());
	SetActorRotation(NewRotation);
}

void AProjectile::setProjectileSpeed()
{
	ProjectileMovement->InitialSpeed = m_fProjectileSpeed;
	ProjectileMovement->MaxSpeed = m_fProjectileSpeed;
}
