// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Squad.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridManager.generated.h"

UENUM(BlueprintType)
enum class EEventBoxState : uint8
{
	FrindlyArea UMETA(DisplayName = "FrindlyArea"),
	NeturalArea UMETA(DisplayName = "NeturalArea"),
	EnemyArea UMETA(DisplayName = "EnemyArea"),
	TE_TypeD UMETA(DisplayName = "Type D"),
};


UCLASS()
class SQUAD_API AGridManager : public AActor
{
	GENERATED_BODY()
	

	AGridManager();

protected:

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	void SetEnemyCharacterStatData(class AEnemySquadCharacter* EnemyCharacter, int32 BrunchNum);

	int32 SetEnemyStatRand();

	TSubclassOf<AActor> GetRandObstacleInArray();

	int32 numberOfEnemyCharacter = 0;
	int32 numberOfObstacle = 0;

public:

	void InitGrid();
	void InitGrid_Boss();

	UFUNCTION()
		void DeleteEventBox();

	void SetGridVisible();

	void Spawned_Enemy();
	void Spawned_Enemy_Boss();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* BoxColiision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro, meta = (AllowPrivateAccess = "true"))
		bool IsSetInit = false;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coordinate")
		int32 EventBoxNumber;

	// Grid

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
		TSubclassOf<AActor> GridToSpawn;

	// Enemy

	UPROPERTY(EditDefaultsOnly, Category = "SpawnData")
		TSubclassOf<AActor> EnemyCharacterToSpawn;

	// Obstacle

	UPROPERTY(EditDefaultsOnly, Category = "SpawnData")
		TSubclassOf<AActor> ObstacleToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnData")
		TArray<TSubclassOf<AActor>> ObstacleToSpawnArray;
	   	  
	// EventObject

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		TSubclassOf<AActor> ObjectToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		int32 ObjectToSpawnXPos;

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		int32 ObjectToSpawnYPos;

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		float ObjectToSpawnLocXPos;

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		float ObjectToSpawnLocYPos;

	UPROPERTY(EditDefaultsOnly, Category = "EventObject")
		float ObjectToSpawnLocZPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
		int32 MaxnumberOfEnemyCharacter;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
		int32 MaxnumberOfObstacle;

	// X
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int Width; 

	// Y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int Depth;

	// GridManger 상태(아군,중립,적군)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
	EEventBoxState EBState;

	TArray<AActor*> SpawnGrids;
	TArray<AActor*> SpawnObtacle;

	class ABattleTrigger* parentBattleTrigger;

	TArray<class AGrid*> YGridArray;
	TArray<TArray<class AGrid*>> XGridArray;


private:
	
	
	void Spawned_Obstacle(AActor* Grid, EEventBoxState Pattern);
	int32 Obstacle_XPosCount = 1;
	int32 Obstacle_XPosCount_netural = 5;
	int32 Obstacle_XPosCount_Enemy = 10;
	bool Obstacle_YPosSwitch = false;

	void Spawned_ObstacleCheck();

	void Spawned_EventObject();

	float CorrentionSpawnObstacle =0.f;
	FVector ObstacleLocCorrectionValue;
	FVector ObstacleScaleCorrectionValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")
		int32 gameDifficultyValue;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true")
		int32 XPosLocRandValue;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true")
		int32 Obstacle_YPosRand_First = 0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true")
		int32 Obstacle_YPosRand_Second = 0;

};
