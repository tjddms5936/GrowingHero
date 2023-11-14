// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "GrowingHero/SkillComponent.h"
#include "GrowingHero/MyCharacterController.h"
#include "GrowingHero/UnitBase.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ASkillBase::ASkillBase() :
	m_fHoldingTime{},
	m_nATK{},
	m_nSkillLv{},
	m_bIsDuplicate{}
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	SpawnNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent >(TEXT("SpawnNiagaraParticle"));
	SpawnNiagaraParticle->SetupAttachment(RootComponent);

	DestroyNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent >(TEXT("DestroyNiagaraParticle"));
	DestroyNiagaraParticle->SetupAttachment(RootComponent);


	m_nSkillLv = 0;
}

void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASkillBase::CallRelease()
{
	AMyCharacterController* MyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (MyController == nullptr)
		return;
	(MyController->SkillComponent)->releaseSkill(this);
}

void ASkillBase::Disappear()
{
	Destroy();
}

void ASkillBase::ActivateSkill()
{
}

void ASkillBase::init(int32 nSkillLV, int32 nSkillMP)
{
	m_nSkillLv = nSkillLV;
	m_nConsumeMP = nSkillMP;
}

void ASkillBase::SpanwSkillNiagara_Sound()
{
	SpawnNiagaraParticle->SetActive(true);
	UGameplayStatics::SpawnSound2D(GetWorld(), SpawnSound);
}

void ASkillBase::DestroySkillNiagara_Sound()
{
	SpawnNiagaraParticle->SetActive(false);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyNiagaraParticle->GetAsset(), GetActorLocation());
	UGameplayStatics::SpawnSound2D(GetWorld(), DestroySound);
}

void ASkillBase::setSkillOwner(AUnitBase* pOwner)
{
	m_pSkillOwner = pOwner;
}
