// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerSquadCharacter.h"
#include "BattleTrigger.generated.h"

// 일반or보스 구분용 enum
UENUM(BlueprintType)
enum class EBattleTriggerState : uint8
{
	Normal UMETA(DisplayName = "Normal"), 
	Boss UMETA(DisplayName = "Boss"),
};

// 좌표 정보 구조체
USTRUCT()
struct FCoordinate {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY()
		int32 XPos;

	UPROPERTY()
		int32 YPos;

	
	UPROPERTY()
	AActor* pGrid = nullptr;

	UPROPERTY()
	AActor* pGrid_X = nullptr;
	UPROPERTY()
	AActor* pGrid_XX = nullptr;
	UPROPERTY()
	AActor* pGrid_Y = nullptr;
	UPROPERTY()
	AActor* pGrid_YY = nullptr;



	void InitCoordinate(int32 Coordinate_XPos, int32 Coordinate_YPos)
	{
		this->XPos = Coordinate_XPos;
		this->YPos = Coordinate_YPos;
	}

	void LogTest() 
	{ 
		UE_LOG(LogClass, Log, L" %f , %f " , XPos, YPos);
	}

	void InitMultiArray()
	{
		//MultiArray.Init(A, 4);
	}

	TArray<FCoordinate> MultiArray;
};

UCLASS()
class SQUAD_API ABattleTrigger : public AActor
{
	GENERATED_BODY()

	ABattleTrigger();

public:	
	// Sets default values for this actor's properties

	void InitBT();

	
public:	
	
	UPROPERTY()
	TArray<AActor*> EnemyList;

	void StartBattleEvent();
	void StartBattleEvent_Boss();
	void InitBattleSetting();
	void EndBattle();	
	
	UFUNCTION()
		void DeleteBattleTrigger();		
	
	void BattleTrigger_PlayerSpreadOut();
	void DeleteEnemyCharacter();

	bool GetEventState();

	FVector GetNeturalAreaLocation();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxColiision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_PlayerAreaCout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_NeutralAreaCout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_EnemyAreaCout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
		float BT_MG_MaxNumberOfObstacle_Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
		float BT_MG_MaxNumberOfObstacle_Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
		float BT_MG_MaxNumberOfObstacle_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
		class AGridManager* pGridManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		FName StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		FName EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		int32 numberOfBox;

	// EventBox = GridManager
	UPROPERTY(EditDefaultsOnly, Category = "Stage")
		TSubclassOf<AActor> EventBoxToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Stage")
		TSubclassOf<AActor> EventSpotToSpawn;

	UPROPERTY(EditAnywhere)
		EBattleTriggerState BTState;


	TArray<FCoordinate> Coordinate;


private:

	TArray<AActor*> SpawnGridManger;
	TArray<APlayerSquadCharacter*> BattleTrigger_FrindlyCharacterList;

	int32 SpawndBoxCout = 0;
	int32 PlayerAreaCout = 0;
	int32 NeutralAreaCout = 0;
	int32 EnemyAreaCout = 0;
	
	AActor* NeturalCenterGrid;

	void SpawnedEventSpot();

	void InitBattleBox();
	void InitBattleBox_Boss();

protected:
	// Called when the game starts or when spawned
	void SetEvent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
		bool eventState; // 1 = battle , 0 = explorer

	class USquadGameInstance* gameIns;


};
