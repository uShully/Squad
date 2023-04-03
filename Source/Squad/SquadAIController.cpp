// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadAIController.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "Grid.h"
#include "Engine/Engine.h"
#include "BattleController.h"
#include "SquadGameInstance.h"
#include "PlayerSquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "SquadCameraManager.h"
#include "CharacterAnimInstance.h"
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
		ReceiveMoveCompleted.AddDynamic(this, &ASquadAIController::Simple);
		
	}

	
}

void ASquadAIController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (PlayerChar != nullptr)
		PlayerCharater_Move();
}

void ASquadAIController::PlayerCharater_Move()
{
	Camera = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	
	if(Camera->ControlValue_PlayerCharacterMovement_BeginPlay)
	{
		float a = PlayerChar->numbering;
		FVector Loc = (Camera->UnitPos_First->GetComponentLocation());
		FVector Loc2 = FVector(180 * a, 0.f, 0.f);

		FVector FinalLocation = Loc + Loc2;

		MoveToLocation(FinalLocation , (-1.f), false);
		//UE_LOG(LogClass, Log, TEXT(" Loc : %s , numbering : %d"), *FinalLocation.ToString(), PlayerChar->numbering);
	}
	
}

void ASquadAIController::PlayerCharater_MoveLoc(FVector Loc)
{
	MoveToLocation(Loc , (-1.f), false);
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
			tempGrid->GridInfo.GOTO = EGridOntheObject::Normal;
			tempGrid->SetGridInfo_Material();
			BehindGrid->GridInfo.GOTO = EGridOntheObject::Player;
			BehindGrid->SetGridInfo_Material();
			PlayerChar->SetUnderGrid(BehindGrid);

			//PlayerChar->Character_BattleRotator_Start = true;
			//PlayerChar->BattleLocation = SpreadLoc;
			if (BehindGrid->YPos == 3 || BehindGrid->YPos == 4)
				PlayerChar->SpreadOutDirection = true;
			else if (BehindGrid->YPos == 0 || BehindGrid->YPos == 1)
				PlayerChar->SpreadOutDirection = false;

			PlayerChar->GetCharacterMovement()->MaxWalkSpeed = 600.f;
			PlayerChar->GetCharacterMovement()->MaxAcceleration = 400.f;

			Cast<UCharacterAnimInstance>(PlayerChar->animInstance)->Call_GetSpreadOutDirection();

			MoveToLocation(SpreadLoc, (-1.f), false);
			Cast<UCharacterAnimInstance>(PlayerChar->animInstance)->IsSpreadOut = true;
			Cast<UCharacterAnimInstance>(PlayerChar->animInstance)->Call_GetIsSpreadOut();

			testbool = true;			
			
			
		
			
		}
	}
}

void ASquadAIController::EnemyCharacter_ActiveAI()
{	
	
	if (EnemyChar->IsStun == true) {
		EnemyChar->Enemy_TurnEnd();
	}
	else 
	EnemyCharacter_ShotAI();
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

void ASquadAIController::EnemyChararacter_SetFrindlyCharacterList(TArray<AActor*> List)
{
	

	if (AliveFrindlyCharacterList.Num() > 0)
		AliveFrindlyCharacterList.Empty();

	AliveFrindlyCharacterList = List;
}

void ASquadAIController::EnemyCharacter_ShotAI()
{
	APlayerSquadCharacter* tempChar;
	
	if(AliveFrindlyCharacterList.Num() > 0)
	{
		//tempChar = Cast<APlayerSquadCharacter>(AliveFrindlyCharacterList[0]);
		
		float FrindlyCharacterNum = AliveFrindlyCharacterList.Num();
		int32 RandCharacterNum = FMath::FloorToInt(FMath::RandRange(0.f, FrindlyCharacterNum));

		tempChar = Cast<APlayerSquadCharacter>(AliveFrindlyCharacterList[RandCharacterNum]);

		EnemyChar->tempTargetCharacter = tempChar;
		EnemyChar->Enemy_ReadytoShot(tempChar);
		EnemyChar->SetShotTarget(tempChar);
	}
}

void ASquadAIController::Simple(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(testbool == true) { 
		if (PlayerChar != nullptr)
			PlayerChar->Character_BattleRotator_Start = true;
	}
	//SetControlRotation
}

void ASquadAIController::DebugFuc()
{
	
	//->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("FIN"));
}