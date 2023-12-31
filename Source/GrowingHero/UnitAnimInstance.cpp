// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnimInstance.h"
#include "UnitBase.h"
#include "CombatComponent.h"

void UUnitAnimInstance::NativeInitializeAnimation()
{
	// BeginPlay()와 같은 역할
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);
		}
	}
	else {
		// Pawn 존재함
		Unit = Cast<AUnitBase>(Pawn);
	}
}

void UUnitAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Unit = Cast<AUnitBase>(Pawn);

		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity(); // 특정 프레임에서 어떤 스피드를 가지고 있는지 반환함
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}
