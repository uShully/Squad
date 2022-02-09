// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "SquadGameInstance.h"
#include "SquadCameraManager.h"
#include "SquadAIController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadAIController : public AAIController
{
	GENERATED_BODY()

		ASquadAIController();

	virtual void Possess(APawn* InPawn) override;
	virtual void Tick(float Delta) override;



	APlayerSquadCharacter* PlayerChar;
	AEnemySquadCharacter* EnemyChar;

	ASquadCameraManager* Camera;

public:

	bool TempBool;
	
	void EnemyCharacter_ActiveAI();
	void EnemyCharacter_ShotAI();
	void EnemyCharacter_BasicAIMove();
	////////////////////////////////

	void PlayerCharater_Move();
	void PlayerCharater_MoveLoc(FVector Loc);

	void PlayerCharacter_SpreadOut();

private:

	bool PlayerPawn = false;
	bool AIPawn = false;

	

};
