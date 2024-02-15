// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "CombatComponent.h"
#include "EnemyCharacter.h"
#include "StatComponent.h"

#include "Components/SphereComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"

AMyCharacter::AMyCharacter() : 
	OverlappingEnemies{},
	BaseTurnRate{},
	BaseLookUpRate{},
	m_bIsAdventureMode{},
	m_nOpenedUICount{}
{
	
	// ƽ �Լ��� �����ϵ��� ����
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ĳ���� Ŭ������ �⺻������ ������ �ִ� ĸ�� �ݶ��̴��� ũ�⸦ �ʱ�ȭ
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// ĳ���Ͱ� ī�޶��� ȸ���� ���� ȸ������ �ʵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ������ �����Ʈ�� ����
	// ĳ���͸� �̵���Ű�� ���� �̵� ����� ���� ĳ������ ������ �ٸ��� ĳ���͸� �̵� �������� �ʴ� 640���� ȸ�� �ӵ��� ȸ����Ų���� �̵�
	// ĳ������ �̵��� ������� ����
	// ������ �� ĳ������ ��ġ�� ����� ��� ���¶�� ����� ������� �ٿ��� ���۵ǵ��� ����	
	// ����̶� ������̼� �޽ø� �ǹ�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// ī�޶� ĳ���Ϳ��Լ� ������ ������ �⵵�� �����ִ� ������ �� ������Ʈ�� �����ϰ� ����
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArmComponent"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 800.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;
	// ĳ���Ͱ� ȸ���� �� ī�޶�� ȸ������ �ʾ����� ������.
	CameraSpringArmComponent->SetUsingAbsoluteRotation(true);
	// ���� �ڵ�δ� �ذ��� �ȵż� ���� �߰�
	CameraSpringArmComponent->bInheritPitch = false;
	CameraSpringArmComponent->bInheritRoll = false;
	CameraSpringArmComponent->bInheritYaw = false;
	// ī�޶�� ĳ������ �Ÿ��� 800���� ����
	// ������ ���� ȸ������ ������ ĳ���͸� �����ٺ����� ����
	// CameraSpringArmComponent->SetRelativeRotation(FRotator(-60.f, 45.0f, 0.0f));
	// ī�޶� ���� ������ �浹 ����� ���� ī�޶�� ĳ������ �Ÿ��� ���� ī�޶� ���� ���� �ʰ� ������ִ� ������Ƽ.
	CameraSpringArmComponent->bDoCollisionTest = false;


	// ī�޶� ������Ʈ�� �����ϰ� ������ �� ������Ʈ�� ���̴� �۾�
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;


	// ���� �ȿ� ������ �������� ����
	CombatRange = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
	CombatRange->SetupAttachment(GetRootComponent());
	CombatRange->InitSphereRadius(m_FStat.AttackRange);

	BaseTurnRate = 65.f;
	BaseLookUpRate = 10.f;

	initStat(100.f);
	m_eCharType = ECharType::E_Hero;

	m_nOpenedUICount = 0;

	
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CombatRange->SetSphereRadius(getUnitStat()->AttackRange);

	m_pMyController = Cast<AMyCharacterController>(GetController());
}

void AMyCharacter::initStat(float MaxHP)
{
	m_FStat.m_MaxHP = MaxHP;
	m_FStat.m_HP = m_FStat.m_MaxHP;
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent); // PlayerInputComponent�� false��� �ڵ� ����

}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMyCharacter::DestroyChar()
{
	Super::DestroyChar();
	FInputModeUIOnly InputModeData;
	m_pMyController->SetInputMode(InputModeData);
}

void AMyCharacter::DieEvent()
{
	GameOverUMG();
}

void AMyCharacter::AttackEnemy(AEnemyCharacter* Enemy)
{
	TArray<AActor*> OverlappingActors{};
	CombatRange->GetOverlappingActors(OverlappingActors);
	for (auto v : OverlappingActors)
	{
		if (!v->ActorHasTag("Enemy"))
		{
			continue;
		}
		AEnemyCharacter* TargetEnemy = Cast<AEnemyCharacter>(v);
		if (TargetEnemy == Enemy)
		{
			CombatComponent->setInit(Enemy, Enemy->getUnitType());
			CombatComponent->Attack();
			break;
		}
	}
}

void AMyCharacter::GainEXT(int32 nEXP)
{
	bool IsLvUp = false;
	while (nEXP > 0)
	{
		getUnitStat()->CurEXP += nEXP;
		nEXP = 0;
		if (getUnitStat()->CurEXP >= getUnitStat()->LvUpEXP)
		{
			nEXP = getUnitStat()->CurEXP - getUnitStat()->LvUpEXP;
			IsLvUp = true;
			LevelUp();
		}
	}

	if (IsLvUp)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), LevelUpSound);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LevelUpEffect, GetActorLocation());
	}
}

void AMyCharacter::LevelUp()
{
	m_pMyController->CharLevelUp();

	getUnitStat()->CurEXP = 0;
	getUnitStat()->LvUpEXP = (int32)((float)getUnitStat()->LvUpEXP * 1.3f);
	getUnitStat()->m_HP = getUnitStat()->m_MaxHP;
	getUnitStat()->m_MP = getUnitStat()->m_MaxMP;
	HPWarningOff();
}

void AMyCharacter::MediateAttackRange(float fAddRangeAmount)
{
	CombatRange->SetSphereRadius(CombatRange->GetScaledSphereRadius() + fAddRangeAmount);
}
