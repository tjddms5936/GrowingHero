// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// Similar to beginplay
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		class AUnitBase* Unit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		class AMyCharacterController* m_pController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool m_bIsEquipped;
};
