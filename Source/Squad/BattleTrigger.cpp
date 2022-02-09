// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTrigger.h"
#include "Components/BoxComponent.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SquadAIController.h"
#include "GridManager.h"
#include "Grid.h"
#include "SquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "Engine/Engine.h"



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
	
	InitBattleBox();
	/*
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), StageName, pGridManagerArray);
	if (pGridManagerArray.Num() > 0)
	{
		pGridManager = Cast<AGridManager>(pGridManagerArray[0]);
	}
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), EnemyName, EnemyTriggerList);
	*/
}

void ABattleTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		if (OtherActor == Cast<APlayerSquadCharacter>(OtherActor))
	{
		if(!OverlapSwitch){
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Player Overlap");
	
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
			
				//BattleTrigger_FrindlyCharacterList.Add(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i]));
			}
			if (pGridManager != nullptr)
				pGridManager->InitGrid();

			OverlapSwitch = true;
		}
	}
}

void ABattleTrigger::InitBattleSetting()
{
	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->BCIns->InitBattleSetting(EnemyList, this);

	for(int32 i = 0 ; i < SpawnBox.Num() ; i++)
	Cast<AGridManager>(SpawnBox[i])->SetGridVisible();
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

	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 900.f, this->GetActorLocation().Y, this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(EventBoxToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnBox.Add(SpawnedActorRef);
		Cast<AGridManager>(SpawnedActorRef)->parentBattleTrigger = this;

		// GridManager = EventBox
		Cast<AGridManager>(SpawnedActorRef)->EventBoxNumber = i;

		if (PlayerAreaCout < Number_PlayerAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeA;
			PlayerAreaCout++;
		}
		else if (NeutralAreaCout < Number_NeutralAreaCout) {
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeB;
			NeutralAreaCout++;

			tempGrid = SpawnedActorRef;
		}
		else if (EnemyAreaCout < Number_EnemyAreaCout){
			Cast<AGridManager>(SpawnedActorRef)->EBState = EEventBoxState::TE_TypeC;
			EnemyAreaCout++;
		}

		//SetEventBoxState(Cast<AGridManager>(SpawnedActorRef)->EBState);

		Cast<AGridManager>(SpawnedActorRef)->InitGrid();
		
	}

}

void ABattleTrigger::DeleteBattleTrigger()
{
	
	for(int i = 0 ; i < SpawnBox.Num() ; i++)
	{
		Cast<AGridManager>(SpawnBox[i])->DeleteEventBox();
	}
	

	Destroy();
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
void ABattleTrigger::Debug_Fuc()
{
	auto SCMIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	
	for (int32 i = 0; i < SCMIns->FriendlyCharList.Num(); i++)
	{
		auto PlayerController = Cast<ASquadAIController>(Cast<APlayerSquadCharacter>(SCMIns->FriendlyCharList[i])->GetController());
		PlayerController->PlayerCharacter_SpreadOut();
	}
}

FVector ABattleTrigger::GetNeturalAreaLocation()
{
	FVector tempGetValue = tempGrid->GetActorLocation();
	
	
	FVector Alpha = Coordinate[0].MultiArray[0].pGrid->GetActorLocation();
	// 인덱스번호 코드 변경해야함
	FVector Tango = Coordinate[14].MultiArray[4].pGrid->GetActorLocation();

	FVector Loc = (Alpha + Tango) / 2;


	return  Loc;
}