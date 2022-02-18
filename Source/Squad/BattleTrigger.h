// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerSquadCharacter.h"
#include "BattleTrigger.generated.h"


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



	void InitCoordinate(int32 XPos, int32 YPos)
	{
		this->XPos = XPos;
		this->YPos = YPos;
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
	
public:	
	// Sets default values for this actor's properties
	ABattleTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxColiision;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AActor*> EnemyTriggerList;

	UPROPERTY()
	TArray<AActor*> EnemyList;

	void StartBattleEvent();
	void InitBattleSetting();

	bool OverlapSwitch = false;

	TArray<AActor*> pGridManagerArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	class AGridManager* pGridManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName StageName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName EnemyName;

	void EndBattle();

	UFUNCTION()
	void InitBattleBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		int32 numberOfBox;

	// EventBox = GridManager
	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	TSubclassOf<AActor> EventBoxToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	TSubclassOf<AActor> EventSpotToSpawn;

	void SpawnedEventSpot();

	UFUNCTION()
		void DeleteBattleTrigger();

	void BattleTrigger_PlayerSpreadOut();

	//UFUNCTION()
	//void SetEventBoxState(EEventBoxState EventBoxState);

	TArray<APlayerSquadCharacter*> BattleTrigger_FrindlyCharacterList;

	void DeleteEnemyCharacter();

private:

	TArray<AActor*> SpawnBox;

	int32 SpawndBoxCout = 0;
	int32 PlayerAreaCout = 0;
	int32 NeutralAreaCout = 0;
	int32 EnemyAreaCout = 0;

public:

	// Coordinate //


	//FCoordinate CoordinateSystem[20][4];
	
	TArray<FCoordinate> Coordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_PlayerAreaCout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_NeutralAreaCout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AreaSetting")
		int32 Number_EnemyAreaCout;

	AActor* tempGrid;
	FVector GetNeturalAreaLocation();

};
