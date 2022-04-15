// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "BattleController.h"
#include "SquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "SquadCameraManager.h"
#include "CommonStruct.h"
#include "SquadGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSquadCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSquadCharacterData() : Level(1), MaxHP(100.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Data")
	float MaxHP;

};

USTRUCT(Atomic, BlueprintType)
struct FPlayerCharacterSlot
{
	GENERATED_BODY()

public:
	//APlayerSquadCharacter* CharacterInfo;
	TArray<FCharacterDataStruct> CharacterDataArry;
	int32 SlotNum;

	int32 CalSlotNum()
	{
		SlotNum = CharacterDataArry.Num();

		return SlotNum;
	}

	int32 MaxSlotNum = 3;
};
/**
 * 
 */
UCLASS()
class SQUAD_API USquadGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public: 
	USquadGameInstance();

	virtual void Init() override;

	//virtual void BeginPlay() override;

	void IncludeCharacterData(FCharacterDataStruct CharacterData);
	void ExcludeCharacterData();
	int32 GetCharacterDataNum();

	

	FPlayerCharacterSlot CharSlot;

//	UPROPERTY()
	//TArray<FPlayerCharacterSlot> CharSlot;

	///////////////

	ABattleController* BCIns;
	ASquadCameraManager* SCMIns;


	AActor* TargetCharacter;
	AActor* SelectedCharacter;

	UFUNCTION()
	void InitInstance();


	
	FSquadCharacterData* GetSquadCharacterData(int32 Level);


	

private:
	UPROPERTY()
		class UDataTable* SquadCharacterTable;

};

