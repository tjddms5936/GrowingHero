// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "InventoryComponent.h"

#include "UMG//UMG_EquipmentFrame.h"
#include "UMG/UMG_EquipmentWindow.h"
#include "UMG/UMG_EquipmentSlot.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent() :
	m_nMaxLength{},
	m_arStrEquipItem{},
	m_arEquipItem{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_nMaxLength = (int)EEquipType::E_MAX;
	m_arStrEquipItem.Reserve(m_nMaxLength);
	m_arStrEquipItem.Init(FInventoryEquipment(), m_nMaxLength);

	m_arEquipItem.Reserve(m_nMaxLength);
	m_arEquipItem.Init(nullptr, m_nMaxLength);

}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadEquipmentFrame();
	m_pMyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_pMyHero = Cast<AMyCharacter>(m_pMyController->GetPawn());
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEquipmentComponent::EnterEquipment(EEquipType eType, FInventoryEquipment FItem, AEquipItem* pEquipItem)
{
	if (m_arEquipItem[(int)eType] != nullptr)
	{
		m_pMyController->InventorySystemComponent->AddItem(EItemType::E_EQUIPMENT, m_arStrEquipItem[(int)eType].ItemName);
		ReleaseEquipment(eType);
	}

	m_arStrEquipItem[(int)eType].CopyStructure(FItem);
	m_arEquipItem[(int)eType] = pEquipItem;

	if (!ensure(EquipmentFrame != nullptr))
		return;
	EquipmentFrame->EquipmentWindow->UpdateEquipmentSlot(eType);
	

	m_pMyHero->getUnitStat()->ATK += m_arStrEquipItem[(int)eType].ATK;
	m_pMyHero->getUnitStat()->DEF += m_arStrEquipItem[(int)eType].DEF;
	m_pMyHero->getUnitStat()->STR += m_arStrEquipItem[(int)eType].STR;
	m_pMyHero->getUnitStat()->DEX += m_arStrEquipItem[(int)eType].DEX;
	m_pMyHero->getUnitStat()->INT += m_arStrEquipItem[(int)eType].INT;
	m_pMyHero->getUnitStat()->LUK += m_arStrEquipItem[(int)eType].LUK;
}

void UEquipmentComponent::ReleaseEquipment(EEquipType eType)
{
	m_pMyHero->getUnitStat()->ATK -= m_arStrEquipItem[(int)eType].ATK;
	m_pMyHero->getUnitStat()->DEF -= m_arStrEquipItem[(int)eType].DEF;
	m_pMyHero->getUnitStat()->STR -= m_arStrEquipItem[(int)eType].STR;
	m_pMyHero->getUnitStat()->DEX -= m_arStrEquipItem[(int)eType].DEX;
	m_pMyHero->getUnitStat()->INT -= m_arStrEquipItem[(int)eType].INT;
	m_pMyHero->getUnitStat()->LUK -= m_arStrEquipItem[(int)eType].LUK;



	m_arStrEquipItem[(int)eType].ClearStructure();
	EquipmentFrame->EquipmentWindow->UpdateEquipmentSlot(eType);
	if (m_arEquipItem[(int)eType] != nullptr)
	{
		m_arEquipItem[(int)eType]->Destroy();
		m_arEquipItem[(int)eType] = nullptr;
	}
}

void UEquipmentComponent::LoadEquipmentFrame()
{
	if (!ensure(EquipmentFrameClass != nullptr))
		return;

	EquipmentFrame = CreateWidget<UUMG_EquipmentFrame>(GetWorld(), EquipmentFrameClass, FName(EquipmentFrameClass->GetName()));
}

