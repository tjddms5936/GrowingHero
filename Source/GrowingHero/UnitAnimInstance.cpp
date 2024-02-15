// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnimInstance.h"
#include "UnitBase.h"
#include "CombatComponent.h"

void UUnitAnimInstance::NativeInitializeAnimation()
{
	// BeginPlay()�� ���� ����
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);
		}
	}
	else {
		// Pawn ������
		Unit = Cast<AUnitBase>(Pawn);
		m_pController = Cast<AMyCharacterController>(Unit->GetController());
	}
}

void UUnitAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);
			m_pController = Cast<AMyCharacterController>(Unit->GetController());
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity(); // Ư�� �����ӿ��� � ���ǵ带 ������ �ִ��� ��ȯ��
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}
