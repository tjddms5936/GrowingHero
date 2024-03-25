// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/MyCharacter.h"
#include "GrowingHero/EnemyCharacter.h"
#include "GrowingHero/CombatComponent.h"

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
	DestroyEvent(); // ������ �ְ� ����Ʈ �Ѹ��� ���. �������Ʈ���� ����
	Super::Disappear();
}

void AProjectile::ActivateSkill()
{
	DetectTarget();
	if (IsValid(m_pTarget) && GetDistanceTo(m_pTarget) <= DetectTargetRange->GetScaledCapsuleHalfHeight())
	{
		m_bIsActivated = true;
		SpanwSkillNiagara_Sound();
		LookAtTarget();
		setProjectileSpeed();
		setHomingTarget(m_pTarget);
		m_pSkillOwner->CombatComponent->setInit(m_pTarget, ECharType::E_Enemy);
		Super::ActivateSkill();
	}
	else
	{
		ActivateFailed();
		return;
	}
	// Target���� �߻�. ���� �Ÿ��� ���� ���� ���϶��? �Ǵ� �̹� Collision �ȿ� Target�� �ִٸ�? �ٷ� ������ �ְ� ����
	if (IsValid(m_pTarget) && m_pTarget->GetDistanceTo(this) <= 200)
	{
		BoomEvent();
	}
}

void AProjectile::init(int32 nSkillLV, int32 nConsumeMP, int32 nSkillNum)
{
	Super::init(nSkillLV, nConsumeMP, nSkillNum);
	m_nATK *= nSkillLV;
	IsInitOK = true;

	ActivateSkill();

	// IsDestroy �ǹ� : Spawn ����  <-> Init '���� ����'�� �ٷ� Target�� Overlap�Ǵ� ��� OverlapBegin�Լ����� Destroy�Ǵ°� ����
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
	// ���� ������ ������ ������ ����
	if (!OtherActor->ActorHasTag("Enemy"))
		return;

	if (m_pTarget && m_pTarget == Cast<AEnemyCharacter>(OtherActor))
	{
		BoomEvent();
	}
}

void AProjectile::ApplyDmg()
{
	// ������ �ִ� ���
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
	// Target ����
	AMyCharacterController* HeroController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (HeroController == nullptr)
		return;

	if (HeroController->m_pClickedProp && HeroController->m_pClickedProp->ActorHasTag("Enemy"))
	{
		m_pTarget = Cast<AEnemyCharacter>(HeroController->m_pClickedProp);
	}

	if (m_pTarget)
		return;

	// �� ĳ���Ͱ� Target�� Ŭ�� ���� ��Ȳ�̴�.
	// �þ� ���� ������ ���� ����� ���� ã�� �˰����� �ʿ��ϴ�.
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

void AProjectile::BoomEvent()
{
	DestroySkillNiagara_Sound();
	ApplyDmg();
	if (IsInitOK)
	{
		Disappear();
	}
	IsDestroy = true;
}

void AProjectile::ActivateFailed()
{
	m_pSkillOwner->getUnitStat()->m_MP += m_nConsumeMP;
	UE_LOG(LogTemp, Warning, TEXT("ActivateFailed"));
}
