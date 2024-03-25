// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	// �� Ŭ������ �ؾ� �ϴ� ����� �ܼ��ϴ�. �κ��丮 �迭�� ����ְ�
	// �迭�� �������� �ִ� ���
	// UI�� ��ȣ�ۿ��ϴ� ���
	// �迭���� �������� ���� ��� 
	// ���� �迭�� FName���� �Ǿ��ִ� ������ �̸� �迭�̴�.
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void initInventory();

	// �Լ� �����Ϳ� ����� �Լ�
	bool AddItem_Consumption(FName ItemRowName);
	bool AddItem_Equipment(FName ItemRowName);
	void SwapSlot_Consumption(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void SwapSlot_Equipment(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem_Consumption(FName ItemName, int32 SlotIndex);
	void ClearItem_Equipment(FName ItemName, int32 SlotIndex);
	void SubtractItem_Consumption(FName ItemName, int32 SlotIndex);
	void SubtractItem_Equipment(FName ItemName, int32 SlotIndex);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> m_arConsumptionItem; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryEquipment> m_arEquipmentItem;

	// �Һ�������� ���... �������� �����ҰŴϱ� �Ź� ã�°� ��ȿ������. 
	// �ٸ� ���� ����ó�� �����ϸ� ���� �������� ���״�... �뷮 �� �� ������ �Ѱ��� ������ �� Slot�� ���� �� ������ �� ������ �߰� �ȵ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FName, int32> m_mapConsumption_SameItem;

	// ������ ���� ã�Ƽ� ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	class UDataTable* ConsumptionItemDB; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	class UDataTable* EquipmentItemDB; 

public:
	int32 getMaxInventoryLength();
	bool AddItem(enum class EItemType eItemType, FName ItemRowName);
	void swapSlot(enum class EItemType eItemType, int32 nDragSlotIndex, int32 nDropSlotIndex);
	void ClearItem(enum class EItemType eItemType ,FName ItemName, int32 SlotIndex);
	void SubtractItem(enum class EItemType eItemType ,FName ItemName, int32 SlotIndex);

	// �Լ�������
	bool(UInventoryComponent::* m_arAddItem[(int)EItemType::E_MAX])(FName ItemRowName);
	void(UInventoryComponent::* m_arSwapSlot[(int)EItemType::E_MAX])(int32 nDragSlotIndex, int32 nDropSlotIndex);
	void(UInventoryComponent::* m_arClearItem[(int)EItemType::E_MAX])(FName ItemName, int32 SlotIndex);
	void(UInventoryComponent::* m_arSubtractItem[(int)EItemType::E_MAX])(FName ItemName, int32 SlotIndex);

public:
	// UMG
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UUserWidget> InventoryFrameClass;
	class UUMG_InventoryFrame* InventoryFrame;

	void init();
};
