// Fill out your copyright notice in the Description page of Project Settings.

#include "GridManager.h"
#include "Obstacle.h"
#include "EnemySquadCharacter.h"
#include "Grid.h"
#include "BattleTrigger.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 5.f));
	BoxColiision->SetCollisionProfileName("BoxTrigger");
	BoxColiision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	

}

void AGridManager::InitGrid()
{
	
	FActorSpawnParameters SpawnParams;

	
	if (SpawnGrids.Num() > 0)
	{
		for (int i = 0; i < SpawnGrids.Num(); i++)
		{
			SpawnGrids[i]->Destroy();
		}
	}
	
	int32 Interval_X = 900 / Width;
	int32 Interval_Y = 900 / Depth;

	if (EBState == EEventBoxState::TE_TypeC)
		Obstacle_XPosCount = 10;

	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Depth; j++)
		{
			// 그리드 생성
			FVector Loc(this->GetActorLocation().X - i * Interval_X, this->GetActorLocation().Y + j * Interval_Y, this->GetActorLocation().Z);
						
			AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(GridToSpawn, Loc, this->GetActorRotation(), SpawnParams);
			SpawnGrids.Add(SpawnedActorRef);
			Cast<AGrid>(SpawnedActorRef)->parentEventBox = this;
			
			AGrid* SpawnGrid = Cast<AGrid>(SpawnedActorRef);
			SpawnGrid->XPos = i + (EventBoxNumber * 5);
			SpawnGrid->YPos = j;
			SpawnGrid->GridInfo.GOTO = EGridOntheObject::Normal;

			//

			if (EBState == EEventBoxState::TE_TypeA)
			{
				SpawnGrid->GridState = EGridState::TE_OptionA;
				Spawned_Obstacle(SpawnGrid, EBState);
				Spawned_ObstacleCheck();
			}
			else if (EBState == EEventBoxState::TE_TypeB)
			{
				SpawnGrid->GridState = EGridState::TE_OptionB;
				
				
			}
			else if (EBState == EEventBoxState::TE_TypeC)
			{
				SpawnGrid->GridState = EGridState::TE_OptionC;
				Spawned_Obstacle(SpawnGrid, EBState);
			
			}

			Cast<AGrid>(SpawnedActorRef)->SetGridState();

			SpawnGrid->SetVisible();
			SpawnGrid->InitGrid();
		}
	}
	
	Spawned_Enemy();
}


void AGridManager::DeleteEventBox()
{
	for (int i = 0; i < SpawnGrids.Num(); i++)
	{
		Cast<AGrid>(SpawnGrids[i])->DeleteGrid();
	}

	Destroy();
}

void AGridManager::SetGridVisible()
{
	for(int32 i = 0 ; i < SpawnGrids.Num() ; i++)
	Cast<AGrid>(SpawnGrids[i])->SetVisible();
}

void AGridManager::Spawned_Enemy()
{
	FActorSpawnParameters SpawnParams;
	TArray<AActor*> tempArray;

	for (int32 XPos =10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)
	{
		for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++)
		{
			AActor* tempActor = parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid;

			if (Cast<AGrid>(tempActor)->GridInfo.GOTO == EGridOntheObject::Obstacle)
			{
				tempArray.Add(tempActor);
			}
		}
	}


	for (int32 i = 0; i < tempArray.Num(); i++)
	{
		auto pXPos = Cast<AGrid>(tempArray[i])->GridInfo.XPos;
		auto pYPos = Cast<AGrid>(tempArray[i])->GridInfo.YPos;


		if (pXPos < 14)
		{
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid)->GridInfo.GOTO != EGridOntheObject::Obstacle)
			{
				auto temp = parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid;
				AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn, temp->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
				Cast<AGrid>(temp)->GridInfo.GOTO = EGridOntheObject::Enemy;
				Cast<AGrid>(temp)->SetGridInfo_Material();
				numberOfEnemyCharacter++;
				
			}
		}
	}
	
	if (numberOfEnemyCharacter != MaxnumberOfEnemyCharacter)
	{
		do {
			for (int32 XPos = 10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)
			{
				for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++)
				{
					AGrid* CastGrid = Cast<AGrid>(parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid);
					
					if (CastGrid->GridInfo.GOTO == EGridOntheObject::Normal && numberOfEnemyCharacter <= MaxnumberOfEnemyCharacter - 1 && FMath::RandRange(0, 100) < 10)
					{
						AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn, parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid->GetActorLocation(), this->GetActorRotation(), SpawnParams);
						parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
						CastGrid->GridInfo.GOTO = EGridOntheObject::Enemy;
						CastGrid->SetGridInfo_Material();
						numberOfEnemyCharacter++;
						

					}
				}
			}
		} while (numberOfEnemyCharacter == MaxnumberOfEnemyCharacter - 1);
	}
	
}

void AGridManager::Spawned_Obstacle(AActor* Grid, EEventBoxState Pattern)
{
	FActorSpawnParameters SpawnParams;
	auto DCGrid = Cast<AGrid>(Grid);

	if(Pattern == EEventBoxState::TE_TypeA)
	{
		if(DCGrid->XPos != 0 && DCGrid->YPos != 2)
		{ 	
			if(DCGrid->XPos == Obstacle_XPosCount)
			{ 
				if(Obstacle_XPosCount % 2 == 0 && (DCGrid->YPos == 0 || DCGrid->YPos == 1))
				{
					if (FMath::RandRange(0, 100) < 80.0f &&  !(numberOfObstacle == MaxnumberOfObstacle))
					{
						AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(ObstacleToSpawn, Grid->GetActorLocation(), this->GetActorRotation(), SpawnParams);
						SpawnObtacle.Add(SpawnedObstacleRef);
						Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
						Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
						DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
						DCGrid->SetGridInfo_Material();

						numberOfObstacle++;
						Obstacle_XPosCount++;						
					}

				}
				else if(Obstacle_XPosCount % 2 == 1 && (DCGrid->YPos == 3 || DCGrid->YPos == 4))
				{
					if (FMath::RandRange(0, 100) < 80.0f &&  !(numberOfObstacle == MaxnumberOfObstacle))
					{
						AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(ObstacleToSpawn, Grid->GetActorLocation(), this->GetActorRotation(), SpawnParams);
						SpawnObtacle.Add(SpawnedObstacleRef);
						Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
						Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
						DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
						DCGrid->SetGridInfo_Material();

						numberOfObstacle++;
						Obstacle_XPosCount++;					
					}
				}
			}
		}
	}
	else if (Pattern == EEventBoxState::TE_TypeC)
	{
		if (DCGrid->XPos != 14 && DCGrid->YPos != 2)
		{
			if (DCGrid->XPos == Obstacle_XPosCount)
			{
				if (FMath::RandRange(0, 100) < 50.0f && !(numberOfObstacle == MaxnumberOfObstacle))
				{
					AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(ObstacleToSpawn, Grid->GetActorLocation(), this->GetActorRotation(), SpawnParams);
					SpawnObtacle.Add(SpawnedObstacleRef);
					Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
					Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->YPos;

					DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
					DCGrid->SetGridInfo_Material();
					numberOfObstacle++;
					Obstacle_XPosCount++;
				}
			}
		}
	}


	
}
 
void AGridManager::Spawned_ObstacleCheck() // 아군 진영만
{
	TArray<AActor*> tempArray;
	for (int32 XPos = 0; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)
	{
		for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++)
		{
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid)->GridInfo.GOTO == EGridOntheObject::Obstacle)
			{
				AActor* tempActor = parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid;
				tempArray.Add(tempActor);
			}
		}
	}

	for (int32 i = 0; i < tempArray.Num(); i++)
	{
		auto tempGridXPos = Cast<AGrid>(tempArray[i])->XPos;
		auto tempGridYPos = Cast<AGrid>(tempArray[i])->YPos;

		Cast<AGrid>(parentBattleTrigger->Coordinate[tempGridXPos - 1].MultiArray[tempGridYPos].pGrid)->GridInfo.GOTO = EGridOntheObject::Behind;
	}
}