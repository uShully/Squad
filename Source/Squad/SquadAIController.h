// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/TimeLineComponent.h"
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



	class APlayerSquadCharacter* PlayerChar;
	class AEnemySquadCharacter* EnemyChar;

	class ASquadCameraManager* Camera;

public:

	bool TempBool;
	
	void EnemyCharacter_ActiveAI();
	void EnemyCharacter_ShotAI();
	void EnemyCharacter_BasicAIMove();
	void EnemyChararacter_SetFrindlyCharacterList(TArray<AActor*> List);

	////////////////////////////////

	void PlayerCharater_Move();
	void PlayerCharater_MoveLoc(FVector Loc);

	void PlayerCharacter_SpreadOut();

	TArray<AActor*> AliveFrindlyCharacterList;

	UFUNCTION()
	void Simple(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	void DebugFuc();


public:
	UPROPERTY()
		UCurveVector* Curve1;
	UPROPERTY()
		UCurveVector* Curve2;

	FTimeline LerpTimeline;

	UPROPERTY()
		float LerpTimelineLegth;


private:

	bool PlayerPawn = false;
	bool AIPawn = false;
	bool Spreadbool = false;
	
};
