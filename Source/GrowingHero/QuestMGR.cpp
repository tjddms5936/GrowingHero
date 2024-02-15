// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMGR.h"
#include "MyCharacter.h"
#include "MyCharacterController.h"
#include "StatComponent.h"

// Sets default values for this component's properties
UQuestMGR::UQuestMGR()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestMGR::BeginPlay()
{
	Super::BeginPlay();
	// ...
}


// Called every frame
void UQuestMGR::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestMGR::AddQuest(int32 QuestID)
{
	// DB에서 QuestID에 해당하는 퀘스트 정보를 TMap에 넣어준다. 
	FString RowName = FString::FromInt(QuestID);
	if ((QuestDB->FindRow<FQuestInfo>(FName(*RowName), RowName) != nullptr))
	{
		FQuestInfo QInfo = *(QuestDB->FindRow<FQuestInfo>(FName(*RowName), RowName));
		m_ManagedQuest.Add(QuestID , QInfo);

		if (ED_QuestAdded.IsBound() == true)
			ED_QuestAdded.Broadcast(QuestID);

		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Red, FString::Printf(TEXT("Add Quest!")));
	}
}

void UQuestMGR::RemoveQuest(int32 QuestID)
{
	// TMap에서 QuestID에 해당하는 것을 삭제한다. 
	m_ManagedQuest.Remove(QuestID);

	if (ED_QuestRemoved.IsBound() == true)
		ED_QuestRemoved.Broadcast(QuestID);
}

void UQuestMGR::CompleteQuest(int32 QuestID)
{
	m_CompletedQuest.Add(QuestID);
	CompensateQuest(QuestID);
	
	// TMap에서 QuestID에 해당하는 것을 완료시킨다.
	if (ED_QuestCompleted.IsBound() == true) 
		ED_QuestCompleted.Broadcast(QuestID);

	// 보상해주기. 

	RemoveQuest(QuestID);
	
	
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.5f, FColor::Red, FString::Printf(TEXT("Complete Quest!")));
}

void UQuestMGR::ProgressingQuest(int32 QuestID)
{
	// TMap에서 QuestID에 해당하는 것의 진행도를 1 올려준다.
	FQuestInfo* Q = m_ManagedQuest.Find(QuestID);
	if (Q != nullptr)
	{
		Q->Cur_Progress++;
		if (Q->Cur_Progress >= Q->Max_Progress)
			CompleteQuest(QuestID);
	}
}

bool UQuestMGR::FindCompletedQuest(int32 QuestID)
{
	if(m_CompletedQuest.Find(QuestID) != NULL)
		return true;
	return false;
}

void UQuestMGR::CompensateQuest(int32 QuestID)
{
	FQuestInfo* Q = m_ManagedQuest.Find(QuestID);
	int32 CompenEXP = Q->Compensation_Exp;
	

	if (!IsValid(m_pMyHero) || !IsValid(m_world))
		return;

	m_pMyHero->GainEXT(CompenEXP);

	for (auto m : Q->Compensation_Item)
	{
		// 아이템 필드로 스폰
		FTransform SpawnTransform{};
		SpawnTransform = m_pMyHero->GetMesh()->GetSocketTransform(FName("DropItemPoint"));
		for (int32 i = 0; i < m.Value; i++)
		{
			AInterfaceProp_Inventory* SpawnItem = m_world->SpawnActor<AInterfaceProp_Inventory>(m.Key.Get(), SpawnTransform);
			if (SpawnItem)
			{
				SpawnItem->pickup();
			}
		}
	}
}

