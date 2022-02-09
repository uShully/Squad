// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "BattleTrigger.h"
#include "BattleController.generated.h"


UCLASS()
class SQUAD_API ABattleController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleController();

	void InitBattleSetting(TArray<AActor*> EnemyList, AActor* triggerBox);
	ABattleTrigger* pTriggerBox = nullptr;

	TArray<AActor*> FriendlyCharacters;
	TArray<AActor*> EnemyCharacters;
	TArray<AActor*> AllCharacters;

	void GetFriendlyCharacters();
	void GetEnemyCharacters(TArray<AActor*> EnemyList);
	void SortCharacters();

	void ClearArray();

	void StartTurnSystem();

	void EndTurnSystem();
	void EndTurnSystem_Enemy();


	void SetEnd();

	void SetSelectedCharacter(ASquadCharacter* SelectedCharacter);
	void ClearSelectedCharacter();

	struct BattleSystemStat {
		
		int32 CharNumber;
		int32 TurnNumber;

		// HaveTurnCharacter를 SelectedCharacter로 변경
		AActor* SelectedCharacter;

		void init() { CharNumber = 0; TurnNumber = 0; SelectedCharacter = nullptr; }

	};
	
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	BattleSystemStat SystemState;

	void WorkEnemyAI();
	void ResultBattle();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString TargetName;

	bool IsBattleStart = false;

	bool SetDebug = false;


	void BeCheck();
	
	UFUNCTION()
	void AddEnemyDeathCount();
	
	int32 EnemyDeathCount = 0;


	bool WhosTurn = false; // false = 아군 true = 적군


};
