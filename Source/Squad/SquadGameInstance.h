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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerSquadCharacter* CharacterInfo;



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

	// 삭제 예정 //
	typedef struct PlayerCharSlot
	{
		APlayerSquadCharacter* CharacterInfo;
		int32 SlotNumber;
	} FSlot;

	///////////////

	ABattleController* BCIns;
	ASquadCameraManager* SCMIns;


	AActor* TargetCharacter;
	AActor* SelectedCharacter;

	UPROPERTY()
	TArray<FPlayerCharacterSlot> CharSlot;
	
	FSquadCharacterData* GetSquadCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* SquadCharacterTable;

};

