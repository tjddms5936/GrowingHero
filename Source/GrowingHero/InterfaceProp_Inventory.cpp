// Fill out your copyright notice in the Description page of Project Settings.


#include "InterfaceProp_Inventory.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


#include "InterfaceSystem/UISC_MGR.h"
#include "UnitBase.h"
#include "MyCharacterController.h"
#include "InventoryComponent.h"
#include "MyCharacter.h"

AInterfaceProp_Inventory::AInterfaceProp_Inventory() :
	Super()
{

}

void AInterfaceProp_Inventory::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapBegin);
	m_pCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapEnd);
}

void AInterfaceProp_Inventory::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(m_pMyController->getClickedActor() == this)
		pickup();
}

void AInterfaceProp_Inventory::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AInterfaceProp_Inventory::ActivateSystem()
{
	Super::ActivateSystem();
}

void AInterfaceProp_Inventory::NotifyActorOnClicked(FKey PressedButton)
{
	Super::NotifyActorOnClicked(PressedButton);
	if (m_pInterfacedUnit)
	{
		pickup();
	}
}

void AInterfaceProp_Inventory::pickup()
{
	if (m_pMyController)
	{
		if (!(m_pMyController->InventorySystemComponent->AddItem(m_eItemType, m_ItemRowName)))
			GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("Inventory Full")));

		else
		{
			m_pMyController->setClickedActor(nullptr);
			AInterfaceProp::DestroyActor();
		}
	}
}


