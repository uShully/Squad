// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SquadCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "SquadGameMode.generated.h"

UCLASS(minimalapi)
class ASquadGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASquadGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> BattleStartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> DecisionWidgetClass;


	UPROPERTY()
	UUserWidget* CurrentWidget;

	

public:

	void StartBattle();
	void EndBattle();

	AActor* BTIns = nullptr;

	void ViewDecisionWidget();
	void ViewBattleWidget();

	TArray<AActor*> FieldCharaters;

	void ChangeTargetName(FString Name);
	void ChangeCurrentCharName(FString Name);
	void ChangeWhosTurnName(bool Name);
};



