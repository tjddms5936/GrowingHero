// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"

UUMG_MainMenu::UUMG_MainMenu(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

bool UUMG_MainMenu::Initialize()
{
	bool Success = Super::Initialize(); // 혹시 Initialize가 false를 반환 할 수 있으므로
	if (!Success) return false;
	
	if (!ensure(ExitBtn != nullptr))
		return false;
	ExitBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::ExitGame);
	
	return true;
}


void UUMG_MainMenu::GameLoadMenuOpen()
{
	if (!ensure(MenuSwitcher != nullptr)) 
		return;
	if (!ensure(LoadMenu != nullptr)) 
		return;

	MenuSwitcher->SetActiveWidget(LoadMenu);
}

void UUMG_MainMenu::GameGuideMenuOpen()
{
	if (!ensure(MenuSwitcher != nullptr))
		return;
	if (!ensure(GuideMenu != nullptr))
		return;

	MenuSwitcher->SetActiveWidget(GuideMenu);
}

void UUMG_MainMenu::MenuCancel()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UUMG_MainMenu::GameStart()
{
	
}

void UUMG_MainMenu::LoadGame()
{
	
}

void UUMG_MainMenu::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}