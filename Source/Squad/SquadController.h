// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CursorHighlight.h"
#include "BattleController.h"
#include "Components/TimelineComponent.h"
#include "SquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "PlayerSquadCharacter.h"
#include "SquadCameraManager.h"
#include "SquadController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ASquadController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASquadController();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor", meta = (AllowPrivateAccess = "true"))
	class ACursorHighlight* Highlight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle", meta = (AllowPrivateAccess = "true"))
	class ABattleController* BattleController;

	
	virtual void SetupInputComponent() override;


private:

	FHitResult Hit;
	FVector HitActorLoc;



	void MoveCharacter();
	void MoveToMouseCursor(const FVector Location);

public:

	void SetHighLight(ACursorHighlight* Cursor);

	void Debug_TurnSystem();
	
	FHitResult HitTarget;

	void SetTargetCharacter();

	void CharacterMove();

	void PlayerCharater_Move();


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TimeLine", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* Curve1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TimeLine", Meta = (AllowPrivateAccess = "true"))
	UCurveVector* Curve2;

	FTimeline LerpTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TimeLine", Meta = (AllowPrivateAccess = "true"))
		float LerpTimelineLength;

	UFUNCTION()
	void TimelineCallbackTest(float value);

	UFUNCTION()
	void TimelineFinishCallbackTest();

	UFUNCTION()
	FVector PathFinder();

	FVector StartLocation;
	FVector EndLocation;

	FVector StartLoc;
	FVector EndLoc;
	float YOffset;

	bool testbool;
};
