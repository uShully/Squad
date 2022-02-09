// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridManager.generated.h"

UENUM(BlueprintType)
enum class EEventBoxState : uint8
{
	TE_TypeA UMETA(DisplayName = "Type A"),
	TE_TypeB UMETA(DisplayName = "Type B"),
	TE_TypeC UMETA(DisplayName = "Type C"),
	TE_TypeD UMETA(DisplayName = "Type D"),
};


UCLASS()
class SQUAD_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxColiision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;


public:

	void InitGrid();



	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro, meta = (AllowPrivateAccess = "true"))
		bool IsSetInit = false;

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pro, meta = (AllowPrivateAccess = "true"))
		int32 Xnumber;

	UPROPERTY(EditDefaultsOnly, Category = "Grid")
		TSubclassOf<AActor> GridToSpawn;

	// Enemy

	UPROPERTY(EditDefaultsOnly, Category = "SpawnData")
		TSubclassOf<AActor> EnemyCharacterToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnData")
		TSubclassOf<AActor> ObstacleToSpawn;


	UPROPERTY()
		int32 numberOfEnemyCharacter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
		int32 MaxnumberOfEnemyCharacter;

	UPROPERTY()
		int32 numberOfObstacle = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
		int32 MaxnumberOfObstacle;

	// X
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int Width; 

	// Y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int Depth; 

	// min��ġ ���� �Ʊ� ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int MinRestriction;

	// �Ʊ��������� ���� Max��ġ���� �߸�����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
		int MaxRestriction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Grid")
	EEventBoxState EBState;

	UFUNCTION()
	void DeleteEventBox();

	void SetGridVisible();

public:

	void Spawned_Enemy();

private:
	
	
	void Spawned_Obstacle(AActor* Grid, EEventBoxState Pattern);
	int32 Obstacle_XPosCount = 1;
	bool Obstacle_YPosSwitch = false;

	void Spawned_ObstacleCheck();

public:

	TArray<AActor*> SpawnGrids;
	TArray<AActor*> SpawnObtacle;

public:

	class ABattleTrigger* parentBattleTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coordinate")
	int32 EventBoxNumber;
};
