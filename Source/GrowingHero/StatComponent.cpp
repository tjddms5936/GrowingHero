// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "Blueprint/UserWidget.h"
#include "UMG//UMG_StatFrame.h"
#include "UMG/UMG_StatWindow.h"
#include "MyCharacter.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadStatFrame();
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::LoadStatFrame()
{
	if (!ensure(StatFrameClass != nullptr))
		return;

	StatFrame = CreateWidget<UUMG_StatFrame>(GetWorld(), StatFrameClass, FName(StatFrameClass->GetName()));
}

