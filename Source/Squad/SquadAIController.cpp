// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadAIController.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "Grid.h"
#include "Engine/Engine.h"

ASquadAIController::ASquadAIController()
{
	TempBool = true;

	
}


void ASquadAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (InPawn == Cast<AEnemySquadCharacter>(InPawn))
	{
		AIPawn = true;
		EnemyChar = Cast<AEnemySquadCharacter>(InPawn);
	}
	else if (InPawn == Cast<APlayerSquadCharacter>(InPawn))
	{		
		PlayerPawn = true;
		PlayerChar = Cast<APlayerSquadCharacter>(InPawn);
	}

	
}

void ASquadAIController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (PlayerChar != nullptr )
		PlayerCharater_Move();
}

void ASquadAIController::PlayerCharater_Move()
{
	Camera = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;

	if(Camera->ControlValue_PlayerCharacterMovement)
	{
		float a = (float)PlayerChar->numbering;
		FVector Loc = (Camera->BoxColiision->GetComponentLocation());
		FVector Loc2 = FVector(180 * a, 0.f, 0.f);

		FVector FinalLocation = Loc + Loc2;

		MoveToLocation(FinalLocation);
	}	
}

void ASquadAIController::PlayerCharater_MoveLoc(FVector Loc)
{
	MoveToLocation(Loc);
}

void ASquadAIController::PlayerCharacter_SpreadOut()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	
	AGrid* tempGrid = PlayerChar->GetUnderGrid();
	tempGrid->XPos; // 고정
	tempGrid->YPos; // Y축에서 찾아야함
	
	auto num = Cast<ABattleTrigger>(gameMode->BTIns)->Coordinate[tempGrid->XPos].MultiArray.Num();
	
	AGrid* BehindGrid;
	FVector SpreadLoc;
	
	for(int32 i = 0 ; i < num ; i++ )
	{
		if (Cast<AGrid>(Cast<ABattleTrigger>(gameMode->BTIns)->Coordinate[tempGrid->XPos].MultiArray[i].pGrid)->
			GridInfo.GOTO == EGridOntheObject::Behind)
		{
			BehindGrid = Cast<AGrid>(Cast<ABattleTrigger>(gameMode->BTIns)->Coordinate[tempGrid->XPos].MultiArray[i].pGrid);
			FVector SpreadLoc = BehindGrid->GetActorLocation();
			UE_LOG(LogClass, Log, L" Move Spread2 %s", *SpreadLoc.ToString());
			tempGrid->GridInfo.GOTO = EGridOntheObject::Normal;
			tempGrid->SetGridInfo_Material();
			BehindGrid->GridInfo.GOTO = EGridOntheObject::Player;
			BehindGrid->SetGridInfo_Material();

			MoveToLocation(SpreadLoc);
		}
	}

	

	

	UE_LOG(LogClass, Log, L" Move Spread");
}

void ASquadAIController::EnemyCharacter_ActiveAI()
{
	

}

void ASquadAIController::EnemyCharacter_BasicAIMove()
{
	if (TempBool == true)
	{
		FVector Loc = EnemyChar->GetActorLocation() + FVector(0.0f, 300.0f, 0.0f);
		MoveToLocation(Loc, 1, false);
		TempBool = false;
	}
	else
	{
		FVector Loc = EnemyChar->GetActorLocation() + FVector(0.0f, -300.0f, 0.0f);
		MoveToLocation(Loc, 1, false);
		TempBool = true;
	}
}

void ASquadAIController::EnemyCharacter_ShotAI()
{
	//EnemyChar->GetBelongToBattleTrigger();
}