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


void AInterfaceProp_Inventory::BeginPlay()
{
	Super::BeginPlay();
	m_pCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapBegin);
	m_pCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AInterfaceProp_Inventory::OnOverlapEnd);
}

void AInterfaceProp_Inventory::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (m_pMyController->getClickedActor() == this && !m_bIsInRange)
	{
		pickup();
	}
	m_bIsInRange = true;

	// Super에서 이미 다 걸러서 내 캐릭터로 세팅된 상태
	if(IsValid(m_pInterfacedUnit))
		m_pInterfacedUnit->Fuc_DeleSingle.BindUFunction(this, FName("pickup"));
}

void AInterfaceProp_Inventory::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(m_pInterfacedUnit))
		m_pInterfacedUnit->Fuc_DeleSingle.Unbind();
	m_bIsInRange = false;
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}

void AInterfaceProp_Inventory::ActivateSystem()
{
	Super::ActivateSystem();
}

void AInterfaceProp_Inventory::NotifyActorOnClicked(FKey PressedButton)
{
	Super::NotifyActorOnClicked(PressedButton);
	if (m_pMyController->getClickedActor() == this && m_bIsInRange)
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
			UGameplayStatics::SpawnSound2D(GetWorld(), PickUpSound);
			m_pMyController->m_pClickedProp = nullptr;
			AInterfaceProp::DestroyActor();
		}
	}
}


