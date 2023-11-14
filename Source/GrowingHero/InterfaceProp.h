// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceSystem/UISC_MGR.h"
#include "InterfaceProp.generated.h"

UCLASS(Abstract)
class GROWINGHERO_API AInterfaceProp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInterfaceProp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InterfaceProp | Mesh")
	class UStaticMeshComponent* m_pMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InterfaceProp | Collision")
	class USphereComponent* m_pCollisionVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	class AMyCharacterController* m_pMyController;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InterfaceProp|UISC")
	class AUnitBase* m_pInterfacedUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InterfaceProp|UISC")
	class AUISC_MGR* m_pUISC_MGR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InterfaceProp|UISC")
	TArray<class UUserInterfaceSystemComponent*> m_arSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InterfaceProp|UISC") 
	EItemType m_eItemType;
public:
	// ================ 함수 ===================
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	AUISC_MGR* getUISC_MGR();

	virtual void ActivateSystem();

	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;


	AUnitBase* getInterfacedUnit();
	void setInterfacedUnit(AUnitBase* pInterfacedUnit);
	void DestroyActor();
};
