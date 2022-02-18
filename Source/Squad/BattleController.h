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
	void StartTurnSystem_init();

	// 2/12 �߰�
	TArray<AActor*> PlayerStartBattleArray;
	TArray<AActor*> PlayerEndBattleArray; // ������ ���� �÷��̾� ĳ���͸� ����ִ´�
	TArray<AActor*> tempPlayerEndBattleArray;
	TArray<AActor*> EnemyStartBattleArray;
	TArray<AActor*> EnemyEndBattleArray; // ������ ���� �÷��̾� ĳ���͸� ����ִ´�

	// 2/16 �߰�

	void ControlCharacterCameraMovement(bool PlayerMovementSwitch);

	//

	void RemoveFromPlayerEndBattleArray(int32 ArrayNumbering ,int32 Numbering);

	void AddPlayerEndBattleArray(AActor * Actor);
	
	void RemoveFromEnemyEndBattleArray(int32 ArrayNumbering);
	void ResetPlayerEndBattleArray();

	//
	void EndTurnSystem();
	void EndTurnSystem_Enemy();


	void SetEnd();

	void SetSelectedCharacter(ASquadCharacter* SelectedCharacter);
	void ClearSelectedCharacter();

	struct BattleSystemStat {
		
		int32 CharNumber;
		int32 TurnNumber;

		// HaveTurnCharacter�� SelectedCharacter�� ����
		AActor* SelectedCharacter;

		void init() { CharNumber = 0; TurnNumber = 0; SelectedCharacter = nullptr; }

	};
	
	

	void ResultBattle();

	void ResultBattle_temp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	BattleSystemStat SystemState;

	void WorkEnemyAI();
	

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
	int32 PlayerDeathCount = 0;


	bool WhosTurn = false; // false = �Ʊ� true = ����



	FTimerHandle WaitHandle;
	float WaitTime = 2.f;
};
