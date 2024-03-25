// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
	m_QuestMGR = CreateDefaultSubobject<UQuestMGR>(TEXT("QuestMGR"));
}

