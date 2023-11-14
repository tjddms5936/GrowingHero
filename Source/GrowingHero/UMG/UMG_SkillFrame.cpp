// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_SkillFrame.h"
#include "GrowingHero/MyCharacterController.h"

UUMG_SkillFrame::UUMG_SkillFrame(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	m_nCurSkillPoint{}
{

}

void UUMG_SkillFrame::NativeOnInitialized()
{
	MyController = Cast<AMyCharacterController>(GetWorld()->GetFirstPlayerController());
	if (!ensure(MyController != nullptr))
		return;
}
