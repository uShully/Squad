// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
#include "Blueprint/UserWidget.h"
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

	TArray<AActor*> PreFriendlyCharList;
	TArray<AActor*> tempSCMList;

	void GetFriendlyChar();
	void SetTeamPosition();
	void SortFrindlyCharList();
	void SetCharacterState();
	void SetCharacterMesh(class APlayerSquadCharacter* Character , int32 classNumber);

	// InitGame�� PreInitializeComponents�� ������ � �Լ����� ���� ȣ��ȴ�
	// ���� ��忡�� �Ű� ������ �ʱ�ȭ�ϰ� ����� Ŭ������ �����ϴ� �� ���ȴ�.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> BattleStartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DecisionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DefeatWidgetClass;
	   
	UPROPERTY()
	UUserWidget* CurrentWidget;

	

public:

	TSubclassOf<UUserWidget> GetVictoryWidgetClass();
	TSubclassOf<UUserWidget> GetDefeatWidgetClass();

	void StartBattle();
	void EndBattle();

	AActor* BTIns = nullptr;

	void ViewDecisionWidget();
	void ViewBattleWidget();
	void ViewVictoryWidget();
	void ViewDefeatWidget();

	TArray<AActor*> FieldCharaters;

	void ChangeTargetName(FString Name);
	void ChangeCurrentCharName(FString Name);
	void ChangeWhosTurnName(bool Name);

	void LoadCharacterMesh();

//	ACommonStruct CommonStructIns;
	


};



