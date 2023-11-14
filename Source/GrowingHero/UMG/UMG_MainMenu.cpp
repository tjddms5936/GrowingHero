// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG_MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UUMG_MainMenu::UUMG_MainMenu(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

bool UUMG_MainMenu::Initialize()
{
	bool Success = Super::Initialize(); // 혹시 Initialize가 false를 반환 할 수 있으므로
	if (!Success) return false;

	if (!ensure(GameStartBtn != nullptr)) 
		return false;
	GameStartBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::GameStart);

	if (!ensure(OpenGameLoadMenuBtn != nullptr))
		return false;
	OpenGameLoadMenuBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::GameLoadMenuOpen);
	
	if (!ensure(OpenGameGuidMenueBtn != nullptr))
		return false;
	OpenGameGuidMenueBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::GameGuideMenuOpen);

	if (!ensure(GameLoadMenuBackBtn != nullptr))
		return false;
	GameLoadMenuBackBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::MenuCancel);

	if (!ensure(GameGuideMenuBackBtn != nullptr))
		return false;
	GameGuideMenuBackBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::MenuCancel);

	if (!ensure(LoadGameBtn != nullptr))
		return false;
	LoadGameBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::LoadGame);
	
	if (!ensure(ExitBtn != nullptr))
		return false;
	ExitBtn->OnClicked.AddDynamic(this, &UUMG_MainMenu::ExitGame);
	
	return true;
}


void UUMG_MainMenu::GameLoadMenuOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("GameLoad"));
	if (!ensure(MenuSwitcher != nullptr)) 
		return;
	if (!ensure(LoadMenu != nullptr)) 
		return;

	MenuSwitcher->SetActiveWidget(LoadMenu);
}

void UUMG_MainMenu::GameGuideMenuOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("GameGuide"));
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