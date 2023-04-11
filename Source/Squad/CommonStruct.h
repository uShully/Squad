// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "CommonStruct.generated.h"

// 인스턴스or세이브 파일에 저장시킬 캐릭터 정보
USTRUCT(Atomic, BlueprintType)
struct FCharacterDataStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LifePoint = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ClassNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentAmmo = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsExist = false;

};


USTRUCT(Atomic, BlueprintType)
struct FSkeletalMeshCop
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Cap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Cap_equip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Head_equip1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Head_equip2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Shirt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Pants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Boots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Hands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_outfit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Backpack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Backpack2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_collar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_shoulder_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Vest_shoulder_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Vest_bottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Decals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Radio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Kneepad_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class 	USkeletalMesh* Kneepad_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Holster;


	void InitSkeletalMesh(USkeletalMesh* S_Head, USkeletalMesh* S_Cap, USkeletalMesh* S_Cap_equip
		, USkeletalMesh* S_Head_equip1, USkeletalMesh* S_Head_equip2, USkeletalMesh* S_Shirt
		, USkeletalMesh* S_Pants, USkeletalMesh* S_Boots, USkeletalMesh* S_Hands, USkeletalMesh* S_Vest
		, USkeletalMesh* S_Vest_outfit, USkeletalMesh* S_Backpack, USkeletalMesh* S_Backpack2, USkeletalMesh* S_Vest_collar
		, USkeletalMesh* S_Vest_shoulder_R, USkeletalMesh* S_Vest_shoulder_L, USkeletalMesh* S_Vest_bottom, USkeletalMesh* S_Decals
		, USkeletalMesh* S_Radio, USkeletalMesh* S_Kneepad_R, USkeletalMesh* S_Kneepad_L, USkeletalMesh* S_Holster)
	{
		if (Head != nullptr) this->Head = (S_Head);
		if (Cap != nullptr) this->Cap = (S_Cap);
		if (Cap_equip != nullptr) this->Cap_equip = (S_Cap_equip);
		if (Head_equip1 != nullptr) this->Head_equip1 = (S_Head_equip1);
		if (Head_equip2 != nullptr) this->Head_equip2 = (S_Head_equip2);
		if (Shirt != nullptr) this->Shirt = (S_Shirt);
		if (Pants != nullptr) this->Pants = (S_Pants);
		if (Boots != nullptr) this->Boots = (S_Boots);
		if (Hands != nullptr) this->Hands = (S_Hands);
		if (Vest != nullptr) this->Vest = (S_Vest);
		if (Vest_outfit != nullptr) this->Vest_outfit = (S_Vest_outfit);
		if (Backpack != nullptr) this->Backpack = (S_Backpack);
		if (Backpack2 != nullptr) this->Backpack2 = (S_Backpack2);
		if (Vest_collar != nullptr) this->Vest_collar = (S_Vest_collar);
		if (Vest_shoulder_R != nullptr) this->Vest_shoulder_R = (S_Vest_shoulder_R);
		if (Vest_shoulder_L != nullptr) this->Vest_shoulder_L = (S_Vest_shoulder_L);
		if (Vest_bottom != nullptr) this->Vest_bottom = (S_Vest_bottom);
		if (Decals != nullptr) this->Decals = (S_Decals);
		if (Radio != nullptr) this->Radio = (S_Radio);
		if (Kneepad_R != nullptr) this->Kneepad_R = (S_Kneepad_R);
		if (Kneepad_L != nullptr) this->Kneepad_L = (S_Kneepad_L);
		if (Holster != nullptr) this->Holster = (S_Holster);
	};
};

UENUM(BlueprintType)
enum class EDecisionEnum : uint8
{
	Battle,
	NonBattle,
	Boss,
	Default
};

UENUM(BlueprintType)
enum class EDecisionEventResultEnum : uint8
{
	BattleStart,
	BossBattleStart,
	BattleAvoidance,
	AllUnitDamage,
	TargetUnitDamage,
	AllUnitRecover,
	TargetUnitRecover,
	IncounterEnd,
	Default
};

USTRUCT(Atomic, BlueprintType)
struct FItem
{
	GENERATED_BODY()

public:
	
	FString ItemName;
	int32 ItemNumber;

};

USTRUCT(Atomic, BlueprintType)
struct FIteminventory
{
	GENERATED_BODY()

public:



	//TArray<FItem> ItemArray;
	FItem medicine = { "medicine", 0 };
	FItem Food = { "food" , 0 };

	void InitInventory() { medicine.ItemNumber = 0; Food.ItemNumber = 0; }

	int32 AddItem(FItem* Item, int32 num) { Item->ItemNumber += num; return Item->ItemNumber; }

	int32 GetFoodNum() { return this->Food.ItemNumber; }
	int32 GetmedicineNum() {return this->medicine.ItemNumber;}
};

UCLASS()
class SQUAD_API ACommonStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommonStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};

