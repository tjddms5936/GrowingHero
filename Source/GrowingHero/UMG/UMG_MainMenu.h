// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GrowingHero/GrowingHeroSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UMG_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GROWINGHERO_API UUMG_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UUMG_MainMenu(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* LoadMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* GuideMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenGameLoadMenuBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenGameGuidMenueBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameLoadMenuBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameGuideMenuBackBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadGameBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitBtn;
	
	
public:
	UFUNCTION()
	void GameLoadMenuOpen();

	UFUNCTION()  
	void GameGuideMenuOpen();

	UFUNCTION()
	void MenuCancel();

	UFUNCTION()
	void GameStart();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void ExitGame();
};
