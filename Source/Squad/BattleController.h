// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Squad.h"
#include "GameFramework/Actor.h"
#include "SquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "BattleTrigger.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "BattleController.generated.h"


UCLASS()
class SQUAD_API ABattleController : public AActor
{
	GENERATED_BODY()

	friend class ASquadGameMode;

	ABattleController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	struct BattleSystemStat {

		int32 CharNumber;
		int32 TurnNumber;

		// HaveTurnCharacter를 SelectedCharacter로 변경
		AActor* SelectedCharacter;

		void init() { CharNumber = 0; TurnNumber = 0; }

	};

protected:

	BattleSystemStat SystemState;
	// 캐릭터 배열
	TArray<AActor*> FriendlyCharacters;
	TArray<AActor*> EnemyCharacters;
	TArray<AActor*> AllCharacters;

	// 죽은 카운트
	int32 EnemyDeathCount = 0;
	int32 PlayerDeathCount = 0;

	// 적군 턴 진행 시간
	FTimerHandle WaitHandle;
	float WaitTime = 2.f;

	// 아군/적군 턴 시작/종료 배열
	TArray<AActor*> PlayerStartBattleArray;
	TArray<AActor*> PlayerEndBattleArray; // 전투가 끝난 아군 플레이어 캐릭터를 집어넣는다
	TArray<AActor*> tempPlayerEndBattleArray;
	TArray<AActor*> EnemyStartBattleArray;
	TArray<AActor*> EnemyEndBattleArray; // 전투가 끝난 적 플레이어 캐릭터를 집어넣는다

	// 전투 시작 여부
	bool IsBattleStart = false;

protected:
	// 캐릭터 배열 배치 함수
	void GetFriendlyCharacters();
	void GetEnemyCharacters(TArray<AActor*> EnemyList);
	void SortCharacters();

	// 전투 시작 초기화
	void StartTurnSystem_init();
	void ClearArray();

	// 캐릭터 배열 제어 함수
	void ResetPlayerEndBattleArray();
	void AddPlayerEndBattleArray(AActor * Actor);

	// 캐릭터 쿨타운 변수 제어 함수
	void Update_PlayerCharacterCooldown(APlayerSquadCharacter * playerCharacter);

	// 전투 상태 체크 함수 (캐릭터가 행동을 끝낼때 마다 호출해서 검사)
	void BeCheck();

	void SetIsBattleStart(bool bIsBattle) { IsBattleStart = bIsBattle; };


public:	
	
	// 전투 초기화, 시작 함수
	void InitBattleSetting(TArray<AActor*> EnemyList, AActor* triggerBox);
	void StartTurnSystem();

	// 전투 시 카메라,캐릭터 움직임 제어
	void ControlCharacterCameraMovement(bool PlayerMovementSwitch);
	bool SwitchCharacterMovement(bool PlayerMovementSwitch);
	
	// 캐릭터 죽을시 호출되는 함수
	// 델리게이트로 호출하면 최적화 가능?
	void RemoveFromPlayerEndBattleArray(AActor* RemoveUnit);	
	void RemoveFromEnemyEndBattleArray(AActor* RemoveUnit);
	   
	// 턴 종료 시스템
	void EndTurnSystem();
	void EndTurnSystem_Enemy();

	// 선택된 캐릭터 변수 제어 함수
	void SetSelectedCharacter(ASquadCharacter* SelectedCharacter);
	void ClearSelectedCharacter();
	AActor* GetSelectedCharacter();	
	
	// 전투 종료시 호출되는 함수
	void ResultBattle_bindwidget();
	void ResultBattle_BattleEnd();

	// 턴 시스템, 키 조작 캐릭터 변경 함수
	void ChangeSelectedCharacter();
	void SystemChangeSelectedCharacter();
	void ControlChangeSelectedCharacter();
	void ReverseControlChangeSelectedCharacter();
	
	// 적군 AI 활성화 및 턴 종료
	void ActiveEnemyAI();
	void ActiveEnemyAI_AllCharTurnEnd();
	
	
	// 적 사망시 호출
	UFUNCTION()
		void AddEnemyDeathCount();

	// 캐릭터 강조선 활성화 함수
	void SetSkillTargeting(bool IsAttackSkill);
	void SetDisableSkillTargeting(bool IsAttackSkill);

	bool GetIsBattleStart() { return IsBattleStart; };

public:	

	// 전투 제어 변수
	ABattleTrigger* pBattleTrigger = nullptr;
	bool WhosTurn = false; 	// false = 아군 true = 적군
	bool IsSkillTargeting = false;

	// 디버그 변수
	FString TargetName;
	bool SetDebug = false;

	// Sound
	UPROPERTY(EditAnywhere, Category = "GameSetting")
		class USoundBase* BattleSound;
	UPROPERTY(EditAnywhere, Category = "GameSetting")
		class USoundBase* ExplorerSound;
	UPROPERTY(EditAnywhere)
		class UAudioComponent* BGMComp;

	protected:
		// 게임 인스턴스
		class USquadGameInstance* gameIns;
		// 게임 모드
		class ASquadGameMode* gameMode;
		// 플레이어 컨트롤러
		class ASquadController* SplayerController;
};
