// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "Components/SphereComponent.h"
#include "MyCharacterController.h"
#include "Components/WidgetComponent.h"
#include "MainGameInstance.h"
#include "QuestMGR.h"
#include "StructureCollection.h"

// Sets default values
ANPCBase::ANPCBase() :
	m_FConvOrder{},
	m_arOwnItemClasses{},
	m_nCurConvIndex{},
	m_nDefaultConvIndex{},
	m_NPCName{},
	m_OwnQuestID_Idx{}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_InterfaceRange = CreateDefaultSubobject<USphereComponent>(TEXT("InterfaceRange"));
	m_InterfaceRange->SetupAttachment(GetRootComponent());

	NPCNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCNameWidget"));
	NPCNameWidget->SetupAttachment(RootComponent);

	m_FConvOrder.Reserve(50); // ��ȭ ���� ����
	m_arOwnItemClasses.Reserve(100); // ���� NPC�� ��� ���� ������ ���� 

	m_nCurConvIndex = 0;
	m_nDefaultConvIndex = 0;
	m_OwnQuestID_Idx = -1;
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_pController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	m_InterfaceRange->OnComponentBeginOverlap.AddDynamic(this, &ANPCBase::OnOverlapBegin);
	m_InterfaceRange->OnComponentEndOverlap.AddDynamic(this, &ANPCBase::OnOverlapEnd);
	m_MainQuestMGR = Cast<UMainGameInstance>(GetGameInstance())->m_QuestMGR;
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("ExpGratee"))
	{
		m_pController->ED_NPC_Conversation.AddDynamic(this, &ANPCBase::ConversationStart);
	}
}

void ANPCBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("ExpGratee"))
	{
		m_pController->ED_NPC_Conversation.RemoveDynamic(this, &ANPCBase::ConversationStart);
	}
	
}

void ANPCBase::RelevantQuestProgrssing()
{
	for (auto ID : m_arRelevantQuestID)
	{
		if(ID > 0)
			m_MainQuestMGR->ProgressingQuest(ID);
	}
}

void ANPCBase::AddQuestFuncCall()
{
	if (m_OwnQuestID_Idx < 0)
	{
		return;
	}
	if (!m_arOwnQuestID.IsValidIndex(m_OwnQuestID_Idx))
	{
		return;
	}


	// �̹� �������� �ο��Ǿ��ִٸ� ���ְ� ������
	if (m_MainQuestMGR->m_ManagedQuest.Find(m_arOwnQuestID[m_OwnQuestID_Idx].second) != nullptr)
	{
		m_OwnQuestID_Idx++;
		return;
	}

	// �������� ������ �Ϸ�Ǿ�� �ϴµ�, ���࿡ ������� ���������? 
	if (m_OwnQuestID_Idx > 0 && !m_MainQuestMGR->FindCompletedQuest(m_arOwnQuestID[m_OwnQuestID_Idx - 1].second))
	{
		m_OwnQuestID_Idx--;
	}

	// �������� ���ų�... ������ �Ϸ������� ������ �ο�
	if (m_arOwnQuestID[m_OwnQuestID_Idx].first < 0 || m_MainQuestMGR->FindCompletedQuest(m_arOwnQuestID[m_OwnQuestID_Idx].first))
	{
		m_MainQuestMGR->AddQuest(m_arOwnQuestID[m_OwnQuestID_Idx].second);
		m_OwnQuestID_Idx++;
		return;
	}
}
