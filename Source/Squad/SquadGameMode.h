// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	TArray<AActor*> PlayerCharList;
	TArray<AActor*> PreFriendlyCharList;
	TArray<AActor*> tempSCMList;

	void GetFriendlyChar();
	void preSetCharacterState();
	void SetTeamPosition();
	void SortFrindlyCharList();
	void SetCharacterState();
	void SetCharacterMesh(class APlayerSquadCharacter* Character , int32 classNumber);

	void SaveCharacterData();

	void CheckArrayNum();

	void SetStartGamelevel();

	UFUNCTION(BlueprintCallable)
	void ClearInstanceCharacterStat();

	// InitGame은 PreInitializeComponents를 포함한 어떤 함수보다 먼저 호출된다
	// 게임 모드에서 매개 변수를 초기화하고 도우미 클래스를 생성하는 데 사용된다.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeSubWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
		void ChangeOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass, EDecisionEventResultEnum DecisionResult,  int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);

	void ChangeBossOutputWidget(TSubclassOf<UUserWidget> NewWidgetClass);



protected:

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

	

public:

	TSubclassOf<UUserWidget> GetVictoryWidgetClass();
	TSubclassOf<UUserWidget> GetDefeatWidgetClass();
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	UUserWidget* GetExploreWidget();

	void StartBattle();
	void EndBattle();

	AActor* BTIns = nullptr;

	void ViewDecisionWidget();
	void ViewDecisionBossWidget();
	void ViewDecisionBossResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);
	void ViewDecisionResultWidget(EDecisionEventResultEnum DecisionResult, int32 MainStream, int32 SubStream, int32 SubStreamSuccFail, FText EventResultText, FText EventValueText);
	void ViewBattleWidget();
	void ViewVictoryWidget();
	void ViewDefeatWidget();

	UUserWidget* GetCurrentWidget();

	UUserWidget * GetCurrentSubWidget();

	TArray<AActor*> FieldCharaters;

	void ChangeTargetName(FString Name);
	void ChangeCurrentCharName(FString Name);

	void ChangeMaxAmmoWidgetText(float MaxAmmo);
	void ChangeCurrentAmmoWidgetText(float CurrentAmmo);
	void ChangeSkillButtonWidgetImage(APlayerSquadCharacter * PlayerCharacter);

	void ChangeWhosTurnName(bool Name);

	void UpDateWidgetText(ASquadCharacter* Char);

	

	//void UpDateWidgetText_Right(ASquadCharacter * Char);

	void UpdateWidget_TargetCharacterStatPanel(ASquadCharacter * TargetChar, ASquadCharacter * SelectedChar);
	void UpdataWidget_TargetCharacterPanel(ASquadCharacter * TargetChar);

	void UpDateWidgetDamageGraph(ASquadCharacter * Char);

	void UpDateWidgetDamageGraph_Blank();
		

	void UpDateWidgetText_Right2(ASquadCharacter * Char);

	void UpDataWidgetText_Skill(APlayerSquadCharacter * Char, int32 skillNumber);


	void ClearCharacterInfoWidgetText();

	void ClearCharacterInfoWidgetText_Right();

	void UpDateWidget_SkillPanel(APlayerSquadCharacter * PCharacter, float SkillNum);

	void ClearWidget_SkillPanel();

	void Set_BattleWidgetOpacity(float val);

	void LoadCharacterMesh();

	void ViewGameVictoryWidget();

	UFUNCTION(BlueprintCallable)
	void SetBattleWidgetSkillButtonOpacity_transparent();

//	ACommonStruct CommonStructIns;
	


};



