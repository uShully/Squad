﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTrigger.h"
#include "Components/BoxComponent.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SquadAIController.h"
#include "SquadController.h"
#include "GridManager.h"
#include "Grid.h"
#include "EventSpot.h"
#include "SquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AIController.h"



// Sets default values
ABattleTrigger::ABattleTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	BoxColiision->SetCollisionProfileName("Trigger");

	RootComponent = BoxColiision;
}


void ABattleTrigger::InitBT()
{
	gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
	if (BTState == EBattleTriggerState::Normal) {
		SetEvent();
		InitBattleBox();
	}
	else if (BTState == EBattleTriggerState::Boss) {
		eventState = 1;
		InitBattleBox_Boss();
	}	
}

void ABattleTrigger::StartBattleEvent()
{
	auto SCMIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;		
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto Controller = Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController();
		Cast<ASquadAIController>(Controller)->PlayerCharater_MoveLoc(Coordinate[3 - i].MultiArray[2].pGrid->GetActorLocation()); // 뒤로 가게하는 범인
			
		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->GridInfo.GOTO = EGridOntheObject::Player;
		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->SetGridInfo_Material_Init();
		Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->SetUnderGrid(Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid));
		BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
	}

	if (pGridManager != nullptr)
		pGridManager->InitGrid();
		
	gameIns->BTIns = this;
	gameMode->ViewDecisionWidget();
	Cast<UDecisionWidget>(gameMode->GetCurrentSubWidget())->SetResult(eventState);
}

void ABattleTrigger::StartBattleEvent_Boss()
{
	auto SCMIns = gameIns->SCMIns;
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	   
	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto Controller = Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController();
		Cast<ASquadAIController>(Controller)->PlayerCharater_MoveLoc(Coordinate[3 - i].MultiArray[2].pGrid->GetActorLocation()); // 뒤로 가게하는 범인


		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->GridInfo.GOTO = EGridOntheObject::Player;
		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->SetGridInfo_Material_Init();
		Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->SetUnderGrid(Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid));
		BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
	}

	if (pGridManager != nullptr)
		pGridManager->InitGrid();

	gameIns->BTIns = this;
	gameMode->ViewDecisionBossWidget();
	Cast<UDecisionWidget>(gameMode->GetCurrentSubWidget())->SetBossResult();
}

void ABattleTrigger::InitBattleSetting()
{
	gameIns->BCIns->InitBattleSetting(EnemyList, this);

	for(int32 i = 0 ; i < SpawnGridManger.Num() ; i++)
	Cast<AGridManager>(SpawnGridManger[i])->SetGridVisible();
}

void ABattleTrigger::EndBattle()
{
	Destroy();
}

void ABattleTrigger::InitBattleBox()
{
	BoxColiision->SetBoxExtent(FVector(450.0f * numberOfBox,900.f , 600.f));

	FActorSpawnParameters SpawnParams;

	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 900.f, this->GetActorLocation().Y, this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventBoxToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnGridManger.Add(SpawnedActorRef);

		// GridManager = EventBox
		Cast<AGridManager>(SpawnedActorRef)->parentBattleTrigger = this;
		Cast<AGridManager>(SpawnedActorRef)->EventBoxNumber = i;

		if (PlayerAreaCout < Number_PlayerAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::FrindlyArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Player;
			PlayerAreaCout++;
		}
		else if (NeutralAreaCout < Number_NeutralAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::NeturalArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Neutral; 
			NeutralAreaCout++;

			NeturalCenterGrid = SpawnedActorRef;
		}
		else if (EnemyAreaCout < Number_EnemyAreaCout){
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::EnemyArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Enemy;
			EnemyAreaCout++;
		}

		Cast<AGridManager>(SpawnedActorRef)->InitGrid();		
	}
	SpawnedEventSpot();
}

void ABattleTrigger::InitBattleBox_Boss()
{
	BoxColiision->SetBoxExtent(FVector(450.0f * numberOfBox, 900.f, 600.f));

	FActorSpawnParameters SpawnParams;
	// 시작시 저장하는 정보 
	// 1. EventBox 정보 :: SpawnGridManger 
	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 900.f, this->GetActorLocation().Y, this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventBoxToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnGridManger.Add(SpawnedActorRef);

		// GridManager = EventBox
		Cast<AGridManager>(SpawnedActorRef)->parentBattleTrigger = this;
		Cast<AGridManager>(SpawnedActorRef)->EventBoxNumber = i;

		if (PlayerAreaCout < Number_PlayerAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::FrindlyArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Player;
			PlayerAreaCout++;
		}
		else if (NeutralAreaCout < Number_NeutralAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::NeturalArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Neutral;
			NeutralAreaCout++;

			NeturalCenterGrid = SpawnedActorRef;
		}
		else if (EnemyAreaCout < Number_EnemyAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::EnemyArea;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Enemy;
			EnemyAreaCout++;
		}

		//SetEventBoxState(Cast<AGridManager>(SpawnedActorRef)->EBState);

		Cast<AGridManager>(SpawnedActorRef)->InitGrid_Boss();

	}

	SpawnedEventSpot();
}

void ABattleTrigger::SpawnedEventSpot()
{
	FActorSpawnParameters SpawnParams;

	FVector spotLoc = Coordinate[3].MultiArray[2].pGrid->GetActorLocation() + FVector(-40.f, 0.f,0.f) + FVector(-30.f, 0.f, 0.f);
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventSpotToSpawn, spotLoc, this->GetActorRotation(), SpawnParams);
	Cast<AEventSpot>(SpawnedActorRef)->parentBattleTrigger = this;
}

void ABattleTrigger::DeleteBattleTrigger()
{	
	if(SpawnGridManger.Num() > 0) {
		for(int i = 0 ; i < SpawnGridManger.Num() ; i++)
		{
			Cast<AGridManager>(SpawnGridManger[i])->DeleteEventBox();
		}
	}	

	Destroy();
}

void ABattleTrigger::DeleteEnemyCharacter()
{
	int32 tempListNum = EnemyList.Num();

	for (int32 i = 0; i < tempListNum; i++)
	{
		auto Char = EnemyList.Pop();
		Char->Destroy();
	}
}

void ABattleTrigger::BattleTrigger_PlayerSpreadOut()
{
	auto SCMIns = gameIns->SCMIns;

	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto PlayerController = Cast<ASquadAIController>(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController());
		PlayerController->PlayerCharacter_SpreadOut();	
	}
}

FVector ABattleTrigger::GetNeturalAreaLocation()
{
	FVector tempGetValue = NeturalCenterGrid->GetActorLocation();
	
	FVector PlayerAreaCenter = Coordinate[2].MultiArray[2].pGrid->GetActorLocation();
	FVector NeturalAreaCenter = Coordinate[7].MultiArray[2].pGrid->GetActorLocation();

	FVector CameraLoc = FVector((PlayerAreaCenter.X + NeturalAreaCenter.X) / 2, PlayerAreaCenter.Y, PlayerAreaCenter.Z);

	FVector Alpha = Coordinate[0].MultiArray[1].pGrid->GetActorLocation();
	FVector Tango = Coordinate[14].MultiArray[1].pGrid->GetActorLocation();

	FVector Loc = (Alpha + Tango) / 2;


	return  Loc;
}

void ABattleTrigger::SetEvent() 
{
	float randfloat = FMath::RandRange(0.f, 1.f);
	float totalProb = 0.f;
	
	for(int i = 0 ; i < 6 ; i++) {	
		struct FEventProb* pEventValue = gameIns->GetBattleEventProbData(i);
		totalProb += pEventValue->EventProb;
	}
	
	totalProb = totalProb / 100;

	if (randfloat <= totalProb && randfloat >= 0.f) {
		eventState = 1; // 전투
	}
	else if (randfloat > totalProb && randfloat <= 100.f) {
		eventState = 0; // 비전투
	}	
}

bool ABattleTrigger::GetEventState()
{
	return eventState;
}

