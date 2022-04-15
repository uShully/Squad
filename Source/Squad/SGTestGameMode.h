// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SGTestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASGTestGameMode : public AGameMode
{
	GENERATED_BODY()

		ASGTestGameMode();

public:

	virtual void InitGame(const FString& MapName, const FString& option, FString& ErrorMessage) override;
	virtual void PreInitializeComponents() override;

	TArray<AActor*> PreFriendlyCharList;
	TArray<AActor*> tempSCMList;

	void GetFriendlyChar();
	void SetTeamPosition();
	void SortFrindlyCharList();
	void SetCharacterState();
	void SetCharacterMesh(class APlayerSquadCharacter * Character, int32 classNumber);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BattleStartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DecisionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> DefeatWidgetClass;

};
