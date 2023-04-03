// Fill out your copyright notice in the Description page of Project Settings.

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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	BoxColiision->SetCollisionProfileName("Trigger");

	RootComponent = BoxColiision;

	//BoxColiision->OnComponentBeginOverlap.AddDynamic(this, &ABattleTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ABattleTrigger::BeginPlay()
{
	Super::BeginPlay();
	

	

}

void ABattleTrigger::InitBT()
{
	if (BTState == EBattleTriggerState::Normal) {
		SetEvent();
		InitBattleBox();
	}
	else if (BTState == EBattleTriggerState::Boss) {
		eventState = 1;
		InitBattleBox_Boss();
	}
}

void ABattleTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
		if (OtherActor == Cast<APlayerSquadCharacter>(OtherActor))
		{
			if(!OverlapSwitch){
	
				auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
				gameMode->BTIns = this;
				gameMode->ViewDecisionWidget();

				auto SCMIns =Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
				SCMIns->ControlValue_PlayerCharacterMovement = !SCMIns->ControlValue_PlayerCharacterMovement;

				for(int32 i = 0 ; i < SCMIns->FriendlyCharList.Num(); i++)
				{
					auto Controller = Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController();
					Cast<ASquadAIController>(Controller)->PlayerCharater_MoveLoc(Coordinate[3 - i].MultiArray[2].pGrid->GetActorLocation());
					Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->GridInfo.GOTO = EGridOntheObject::Player;
					Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->SetGridInfo_Material();
					Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->SetUnderGrid(Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid));
			
					BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
				}
				if (pGridManager != nullptr)
					pGridManager->InitGrid();

				OverlapSwitch = true;
		}
	}
	*/
}

void ABattleTrigger::StartBattleEvent()
{
		auto SCMIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
		//SCMIns->ControlValue_PlayerCharacterMovement = !SCMIns->ControlValue_PlayerCharacterMovement;
		
		auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

		

		for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
		{
			auto Controller = Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController();
			Cast<ASquadAIController>(Controller)->PlayerCharater_MoveLoc(Coordinate[3 - i].MultiArray[2].pGrid->GetActorLocation()); // 뒤로 가게하는 범인
			
			
			Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->GridInfo.GOTO = EGridOntheObject::Player;
			Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->SetGridInfo_Material();
			Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->SetUnderGrid(Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid));
			BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
		}

		if (pGridManager != nullptr)
			pGridManager->InitGrid();


		gameMode->BTIns = this;
		gameMode->ViewDecisionWidget();
		Cast<UDecisionWidget>(gameMode->GetCurrentSubWidget())->SetResult(eventState);

	//	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
}

void ABattleTrigger::StartBattleEvent_Boss()
{
	auto SCMIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	//SCMIns->ControlValue_PlayerCharacterMovement = !SCMIns->ControlValue_PlayerCharacterMovement;

	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());



	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto Controller = Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController();
		Cast<ASquadAIController>(Controller)->PlayerCharater_MoveLoc(Coordinate[3 - i].MultiArray[2].pGrid->GetActorLocation()); // 뒤로 가게하는 범인


		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->GridInfo.GOTO = EGridOntheObject::Player;
		Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid)->SetGridInfo_Material();
		Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->SetUnderGrid(Cast<AGrid>(Coordinate[3 - i].MultiArray[2].pGrid));
		BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
	}

	if (pGridManager != nullptr)
		pGridManager->InitGrid();


	gameMode->BTIns = this;
	gameMode->ViewDecisionBossWidget();
	Cast<UDecisionWidget>(gameMode->GetCurrentSubWidget())->SetBossResult();

	//	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
}

void ABattleTrigger::InitBattleSetting()
{
	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->BCIns->InitBattleSetting(EnemyList, this);

	for(int32 i = 0 ; i < SpawnGridManger.Num() ; i++)
	Cast<AGridManager>(SpawnGridManger[i])->SetGridVisible();
}

void ABattleTrigger::EndBattle()
{
	//pGridManager->EndGrid();
	Destroy();
}

void ABattleTrigger::InitBattleBox()
{
	BoxColiision->SetBoxExtent(FVector(450.0f * numberOfBox,900.f , 600.f));

	FActorSpawnParameters SpawnParams;
	// 시작시 저장하는 정보 
	// 1. EventBox 정보 :: SpawnGridManger
	// 2. 
	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 900.f, this->GetActorLocation().Y, this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventBoxToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnGridManger.Add(SpawnedActorRef);

		// GridManager = EventBox
		Cast<AGridManager>(SpawnedActorRef)->parentBattleTrigger = this;
		Cast<AGridManager>(SpawnedActorRef)->EventBoxNumber = i;

		if (PlayerAreaCout < Number_PlayerAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeA;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Player;
			PlayerAreaCout++;
		}
		else if (NeutralAreaCout < Number_NeutralAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeB;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Neutral; 
			NeutralAreaCout++;

			tempGrid = SpawnedActorRef;
		}
		else if (EnemyAreaCout < Number_EnemyAreaCout){
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeC;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Enemy;
			EnemyAreaCout++;
		}

		//SetEventBoxState(Cast<AGridManager>(SpawnedActorRef)->EBState);

		Cast<AGridManager>(SpawnedActorRef)->InitGrid();
		
	}

	// Spawnd event spot

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
		UE_LOG(LogClass, Log, TEXT(" List Num : %d"), EnemyList.Num());
	}
}
/*
void ABattleTrigger::SetEventBoxState(EEventBoxState EventBoxState)
{
	// 임시 세팅 함수

	if(	SpawndBoxCout < 2 ) 
	EventBoxState = EEventBoxState::TE_TypeA;
	else if (SpawndBoxCout == 2)
	EventBoxState = EEventBoxState::TE_TypeB;
	else if (SpawndBoxCout > 2)
	EventBoxState = EEventBoxState::TE_TypeC;


	SpawndBoxCout++;
}
*/
void ABattleTrigger::BattleTrigger_PlayerSpreadOut()
{
	auto SCMIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	


	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto PlayerController = Cast<ASquadAIController>(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController());
		PlayerController->PlayerCharacter_SpreadOut();
		//PlayerController->OnMoveCompleted
	
	}

	

}

void ABattleTrigger::BattleTrigger_PlayerSetRotator()
{

}

FVector ABattleTrigger::GetNeturalAreaLocation()
{
	FVector tempGetValue = tempGrid->GetActorLocation();
	
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
		struct FEventProb* pEventValue = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetBattleEventProbData(i);
		totalProb += pEventValue->EventProb;
	}
	
	totalProb = totalProb / 100;

	if (randfloat <= totalProb && randfloat >= 0.f) {
		eventState = 1; // 전투
	}
	else if (randfloat > totalProb && randfloat <= 100.f) {
		eventState = 0; // 비전투
	}
	
	//eventState = 1;
}

bool ABattleTrigger::GetEventState()
{
	return eventState;
}

void ABattleTrigger::InitBattleBox_Boss()
{
	BoxColiision->SetBoxExtent(FVector(450.0f * numberOfBox, 900.f, 600.f));

	FActorSpawnParameters SpawnParams;
	// 시작시 저장하는 정보 
	// 1. EventBox 정보 :: SpawnGridManger
	// 2. 
	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 900.f, this->GetActorLocation().Y, this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventBoxToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnGridManger.Add(SpawnedActorRef);

		// GridManager = EventBox
		Cast<AGridManager>(SpawnedActorRef)->parentBattleTrigger = this;
		Cast<AGridManager>(SpawnedActorRef)->EventBoxNumber = i;

		if (PlayerAreaCout < Number_PlayerAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeA;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Player;
			PlayerAreaCout++;
		}
		else if (NeutralAreaCout < Number_NeutralAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeB;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Neutral;
			NeutralAreaCout++;

			tempGrid = SpawnedActorRef;
		}
		else if (EnemyAreaCout < Number_EnemyAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeC;
			Cast<AGridManager>(SpawnedActorRef)->MaxnumberOfObstacle = BT_MG_MaxNumberOfObstacle_Enemy;
			EnemyAreaCout++;
		}

		//SetEventBoxState(Cast<AGridManager>(SpawnedActorRef)->EBState);

		Cast<AGridManager>(SpawnedActorRef)->InitGrid_Boss();

	}

	// Spawnd event spot

	SpawnedEventSpot();
}