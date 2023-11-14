// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructureCollection.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GROWINGHERO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 m_nMaxInventoryLength;

	// 이 클래스가 해야 하는 기능은 단순하다. 인벤토리 배열을 들고있고
	// 배열에 아이템을 넣는 기능
	// UI와 상호작용하는 기능
	// 배열에서 아이템을 빼는 기능 
	// 물론 배열은 FName으로 되어있는 아이템 이름 배열이다.
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// 함수 포인터에 연결될 함수
	bool AddItem_Consumption(FName ItemRowName);
	bool AddItem_Equipment(FName ItemRowName);
	void SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem_Consumption(FName ItemName, int32 SlotIndex);
	void ClearItem_Equipment(FName ItemName, int32 SlotIndex);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Inventory")
	TMap<FName, FItemSlotIndex> m_mapInventory_Consumption; // Key : 아이템이름	Value : 해당하는 아이템이 들어있는 Slot의 Index 모음 배열. 2개라면 첫번째는 꽉찬 Slot
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller|Inventory")
	TMap<FName, FItemSlotIndex> m_mapInventory_Equipment;

public:
	int32 getMaxInventoryLength();
	bool AddItem(EItemType eItemType, FName ItemRowName);
	void swapSlot(EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem(EItemType eItemType ,FName ItemName, int32 SlotIndex);

	// 함수포인터
	bool(UInventoryComponent::* m_arAddItem[(int)EItemType::E_MAX])(FName ItemRowName);
	void(UInventoryComponent::* m_arSwapSlot[(int)EItemType::E_MAX])(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void(UInventoryComponent::* m_arClearItem[(int)EItemType::E_MAX])(FName ItemName, int32 SlotIndex);

public:
	// UMG
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UUserWidget> InventoryFrameClass;
	class UUMG_InventoryFrame* InventoryFrame;

	void LoadInventoryFrame();

};
