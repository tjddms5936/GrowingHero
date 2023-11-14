// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceProp_Field.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "InterfaceSystem/UISC_MGR.h"
#include "UnitBase.h"
#include "MyCharacterController.h"

void AInterfaceProp_Field::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInterfaceProp_Field::OnOverlapBegin);
	m_pCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AInterfaceProp_Field::OnOverlapEnd);
}

void AInterfaceProp_Field::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


	if (m_pInterfacedUnit)
	{
		ActivateSystem();
	}
}

void AInterfaceProp_Field::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AInterfaceProp_Field::ActivateSystem()
{
	Super::ActivateSystem();
}

void AInterfaceProp_Field::NotifyActorOnClicked(FKey PressedButton)
{
	Super::NotifyActorOnClicked(PressedButton);
	if (m_pInterfacedUnit)
	{
		ActivateSystem();
	}
}
