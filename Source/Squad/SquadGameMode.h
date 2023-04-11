// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Squad.h"
#include "SquadCharacter.h"
#include "Blueprint/UserWidget.h"
#include "DecisionWidget.h"
#include "GameFramework/GameModeBase.h"
#include "CommonStruct.h"
#include "SquadGameMode.generated.h"

UCLASS(minimalapi)
class ASquadGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASquadGameMode();

	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	// InitGame은 PreInitializeComponents를 포함한 어떤 함수보다 먼저 호출된다
	// 게임 모드에서 매개 변수를 초기화하고 도우미 클래스를 생성하는 데 사용된다.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:

	// 게임 캐릭터 초기화 함수
	void GetFriendlyChar();
	void preSetCharacterState();
	void SetTeamPosition();
	void SortFrindlyCharList();
	void SetCharacterState();
	void SetCharacterMesh(class APlayerSquadCharacter* Character , int32 classNumber);
	void SaveCharacterData();	   
	void SetStartGamelevel();
	UFUNCTION(BlueprintCallable)
		void ClearInstanceCharacterStat();
	
	// 뷰포트 위젯 변경 함수
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void ChangeSubWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void ChangeOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass, EDecisionEventResultEnum DecisionResult,  int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void ChangeBossOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	// 위젯 반환 함수

	TSubclassOf<UUserWidget> GetVictoryWidgetClass();
	TSubclassOf<UUserWidget> GetDefeatWidgetClass();
	
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		UUserWidget* GetExploreWidget();
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		UUserWidget* GetCurrentWidget();
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		UUserWidget * GetCurrentSubWidget();

	// 전투 시작/종료 함수
	void StartBattle();
	void EndBattle();

	// 위젯 변경 함수
	void ViewDecisionWidget();
	void ViewDecisionBossWidget();
	void ViewDecisionBossResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);
	void ViewDecisionResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);
	void ViewBattleWidget();
	void ViewVictoryWidget();
	void ViewDefeatWidget();
	
	// 위젯 텍스트 변경 함수
	void ChangeTargetName(FString Name);
	void ChangeCurrentCharName(FString Name);
	void ChangeMaxAmmoWidgetText(float MaxAmmo);
	void ChangeCurrentAmmoWidgetText(float CurrentAmmo);
	void ChangeSkillButtonWidgetImage(APlayerSquadCharacter * PlayerCharacter);
	void ChangeWhosTurnName(bool Name);

	void UpDateWidgetText(ASquadCharacter* Char);
	void UpdateWidget_TargetCharacterStatPanel(ASquadCharacter * TargetChar, ASquadCharacter * SelectedChar);
	void UpdataWidget_TargetCharacterPanel(ASquadCharacter * TargetChar);
	void UpDateWidgetDamageGraph(ASquadCharacter * Char);
	void UpDateWidgetDamageGraph_Blank();
	void UpDateWidgetText_Right2(ASquadCharacter * Char);
	void UpDataWidgetText_Skill(APlayerSquadCharacter * Char, int32 skillNumber);
	void UpDateWidget_SkillPanel(APlayerSquadCharacter * PCharacter, float SkillNum);

	// 위젯 초기화 함수
	void ClearCharacterInfoWidgetText();
	void ClearCharacterInfoWidgetText_Right();
	void ClearWidget_SkillPanel();
		
	// 게임 승리 위젯
	void ViewGameVictoryWidget();

	// 전투 위젯 투명도 조절 함수
		void Set_BattleWidgetOpacity(float val);
	UFUNCTION(BlueprintCallable)
		void SetBattleWidgetSkillButtonOpacity_transparent();

protected:

	TArray<AActor*> PreFriendlyCharList;
	TArray<AActor*> tempSCMList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BattleStartWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DecisionWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DecisionBossWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DecisionBossResultWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DecisionResultWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> VictoryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DefeatWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> ExploreWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> GameVictoryWidgetClass;
	   
	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		UUserWidget* CurrentSubWidget;

	UPROPERTY()
		UUserWidget* DisplayExplerWidget;

	class USquadGameInstance* gameIns;

protected:
	void CheckArrayNum();
};



