// Fill out your copyright notice in the Description page of Project Settings.

#include "GridManager.h"
#include "Obstacle.h"
#include "EnemySquadCharacter.h"
#include "Grid.h"
#include "BattleTrigger.h"
#include "Math/UnrealMathUtility.h"
#include "SquadGameInstance.h"
#include "Engine/Engine.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxColiision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxColiision->SetBoxExtent(FVector(32.f, 32.f, 5.f));
	BoxColiision->SetCollisionProfileName("BoxTrigger");
	SetRootComponent(BoxColiision);
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	XPosLocRandValue = FMath::RandRange(0.f, 1.999f);
	Obstacle_YPosRand_First = FMath::RandRange(0.f, 1.999f);
	Obstacle_YPosRand_Second = FMath::RandRange(0.f, 1.999f);
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

	YGridArray.Init(nullptr, 5);
	XGridArray.Init(YGridArray, 5);

	if (SpawnGrids.Num() > 0)
	{
		for (int i = 0; i < SpawnGrids.Num(); i++)
		{
			SpawnGrids[i]->Destroy();
		}
	}
	
	int32 Interval_X = 900 / Width;
	int32 Interval_Y = 900 / Depth;

	if (EBState == EEventBoxState::EnemyArea)
		Obstacle_XPosCount = 10;

	// 그리드 생성 로직
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Depth; j++)
		{
			// 그리드 생성
			FVector Loc(this->GetActorLocation().X - i * Interval_X, this->GetActorLocation().Y + j * Interval_Y, this->GetActorLocation().Z - 50.f);
						
			AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(GridToSpawn, Loc, this->GetActorRotation(), SpawnParams);
			SpawnGrids.Add(SpawnedActorRef);
			Cast<AGrid>(SpawnedActorRef)->parentGridManager = this;
			
			// 그리드 정보 초기화
			AGrid* SpawnGrid = Cast<AGrid>(SpawnedActorRef);
			SpawnGrid->XPos = i + (EventBoxNumber * 5);
			SpawnGrid->YPos = j;
			SpawnGrid->GridInfo.GOTO = EGridOntheObject::Normal;

			// 이중 배열(좌표)에 그리드 삽입
			YGridArray[j] = (SpawnGrid);
			if (j == 4) {
				XGridArray[i] = YGridArray;
			}

			// 영역(아군,중립,적) 별로 장애물 생성 
			if (EBState == EEventBoxState::FrindlyArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionA;
				Spawned_Obstacle(SpawnGrid, EBState);
			}
			else if (EBState == EEventBoxState::NeturalArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionB;
				Spawned_Obstacle(SpawnGrid, EBState);				
			}
			else if (EBState == EEventBoxState::EnemyArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionC;
				Spawned_Obstacle(SpawnGrid, EBState);			
			}
			// 그리드 초기화
			// 그리드가 초기화되면서 BattleTriiger의 좌표배열에 정보를 초기화	
			SpawnGrid->SetGridState();
			SpawnGrid->SetVisible();
			SpawnGrid->InitGrid();
		}
	}
	
	// 아군 진영 그리드 상태를 장애물 상태로 변경
	if (EBState == EEventBoxState::FrindlyArea) {
		Spawned_ObstacleCheck();
	}
	// 상위 Battle Tigger의 이벤트 상황이 전투이벤트이면 적군 스폰 
	if (EBState == EEventBoxState::EnemyArea && parentBattleTrigger->GetEventState()) {
		Spawned_Enemy();
	}

	if(EBState == EEventBoxState::NeturalArea) {
		if(parentBattleTrigger->GetEventState()) {
			Spawned_EventObject();
		}
	}
}

void AGridManager::InitGrid_Boss()
{

	FActorSpawnParameters SpawnParams;

	YGridArray.Init(nullptr, 5);
	XGridArray.Init(YGridArray, 5);

	if (SpawnGrids.Num() > 0)
	{
		for (int i = 0; i < SpawnGrids.Num(); i++)
		{
			SpawnGrids[i]->Destroy();
		}
	}

	int32 Interval_X = 900 / Width;
	int32 Interval_Y = 900 / Depth;

	if (EBState == EEventBoxState::EnemyArea)
		Obstacle_XPosCount = 10;

	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Depth; j++)
		{
			// 그리드 생성
			FVector Loc(this->GetActorLocation().X - i * Interval_X, this->GetActorLocation().Y + j * Interval_Y, this->GetActorLocation().Z - 50.f);

			AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(GridToSpawn, Loc, this->GetActorRotation(), SpawnParams);
			SpawnGrids.Add(SpawnedActorRef);
			Cast<AGrid>(SpawnedActorRef)->parentGridManager = this;

			// 그리드 정보 초기화
			AGrid* SpawnGrid = Cast<AGrid>(SpawnedActorRef);
			SpawnGrid->XPos = i + (EventBoxNumber * 5);
			SpawnGrid->YPos = j;
			SpawnGrid->GridInfo.GOTO = EGridOntheObject::Normal;

			// 이중 배열(좌표)에 그리드 삽입
			YGridArray[j] = (SpawnGrid);
			if (j == 4) {
				XGridArray[i] = YGridArray;
			}

			// 영역(아군,중립,적) 별로 장애물 생성 
			if (EBState == EEventBoxState::FrindlyArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionA;
				Spawned_Obstacle(SpawnGrid, EBState);
				Spawned_ObstacleCheck();
			}
			else if (EBState == EEventBoxState::NeturalArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionB;
				Spawned_Obstacle(SpawnGrid, EBState);

			}
			else if (EBState == EEventBoxState::EnemyArea)
			{
				SpawnGrid->GridState = EGridState::TE_OptionC;
				Spawned_Obstacle(SpawnGrid, EBState);

			}

			// 그리드 초기화
			SpawnGrid->SetGridState();
			SpawnGrid->SetVisible();
			SpawnGrid->InitGrid();
		}
	}

	if (parentBattleTrigger->GetEventState()) {
		Spawned_Enemy_Boss();	
	}
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

	// BattleTrigger 좌표 배열에서 적군 영역에 해당하는 좌표중 장애물이 설치된 좌표를 임시배열에 저장
	for (int32 XPos =10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)	{
		for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++)	{
			AActor* tempActor = parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid;

			if (Cast<AGrid>(tempActor)->GridInfo.GOTO == EGridOntheObject::Obstacle) {
				tempArray.Add(tempActor);
			}
		}
	}

	// 장애물이 설치된 좌표 뒤쪽(x+1)에 적 캐릭터를 스폰, 적 캐릭터 정보 초기화
	for (int32 i = 0; i < tempArray.Num(); i++) {
		auto pXPos = Cast<AGrid>(tempArray[i])->GridInfo.XPos;
		auto pYPos = Cast<AGrid>(tempArray[i])->GridInfo.YPos;

		if (pXPos < 14) {
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid)->GridInfo.GOTO != EGridOntheObject::Obstacle) {
				auto temp = parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid;
				// 적캐릭터를 스폰하고 적 캐릭터 배열에 주소값 추가
				AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn,
																			temp->GetActorLocation(),
																			this->GetActorRotation(),
																			SpawnParams);
				parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(temp));
				// 적 캐릭터 정보(병과, 메쉬 등) 초기화
				SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
				// 캐릭터가 밟고 있는 그리드 정보 초기화
				Cast<AGrid>(temp)->GridInfo.GOTO = EGridOntheObject::Enemy;
				Cast<AGrid>(temp)->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
				Cast<AGrid>(temp)->SetGridInfo_Material_Init();
				// 적 캐릭터 검사 변수 증가
				numberOfEnemyCharacter++;				
			}
		}
	}
	
	// 장애물이 충분치 않을때 적 캐릭터가 무작위 위치에 스폰
	if (numberOfEnemyCharacter != MaxnumberOfEnemyCharacter) {
		do {
			for (int32 XPos = 10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)	{
				for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++) {
					AGrid* CastGrid = Cast<AGrid>(parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid);
					
					if (CastGrid->GridInfo.GOTO == EGridOntheObject::Normal 
						&& numberOfEnemyCharacter <= MaxnumberOfEnemyCharacter - 1 
						&& FMath::RandRange(0, 100) < 10) {

						AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn,
						 parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid->GetActorLocation(),
																				this->GetActorRotation(),
																							SpawnParams);
						// 적캐릭터를 스폰하고 적 캐릭터 배열에 주소값 추가
						parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(CastGrid));
						// 적 캐릭터 정보(병과, 메쉬 등) 초기화
						SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
						// 캐릭터가 밟고 있는 그리드 정보 초기화
						CastGrid->GridInfo.GOTO = EGridOntheObject::Enemy;
						CastGrid->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
						CastGrid->SetGridInfo_Material_Init();
						// 적 캐릭터 검사 변수 증가
						numberOfEnemyCharacter++;
					}
				}
			}
		} while (numberOfEnemyCharacter == MaxnumberOfEnemyCharacter - 1); // 적 캐릭터수가 최대치가 될때까지 루프
	}
	
}

void AGridManager::Spawned_Enemy_Boss()
{
	FActorSpawnParameters SpawnParams;
	TArray<AActor*> tempArray;

	for (int32 XPos = 10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)
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


		if (pXPos < 13)
		{
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid)->GridInfo.GOTO != EGridOntheObject::Obstacle)
			{
				auto temp = parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid;
				AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn, temp->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(temp));
				SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
				Cast<AGrid>(temp)->GridInfo.GOTO = EGridOntheObject::Enemy;
				Cast<AGrid>(temp)->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
				Cast<AGrid>(temp)->SetGridInfo_Material_Init();
				numberOfEnemyCharacter++;
			}
		}
		else if (pXPos == 13)
		{
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid)->GridInfo.GOTO != EGridOntheObject::Obstacle)
			{
				auto temp = parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid;
				AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn, temp->GetActorLocation(), this->GetActorRotation(), SpawnParams);
				parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(temp));
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->EnemyRate = EEnemyRate::Boss;
				SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), 1); // 보스 스텟
				Cast<AGrid>(temp)->GridInfo.GOTO = EGridOntheObject::Enemy;
				Cast<AGrid>(temp)->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
				Cast<AGrid>(temp)->SetGridInfo_Material_Init();
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
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(CastGrid));
						SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
						CastGrid->GridInfo.GOTO = EGridOntheObject::Enemy;
						CastGrid->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
						CastGrid->SetGridInfo_Material_Init();
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

	if(Pattern == EEventBoxState::FrindlyArea)
	{
		if((DCGrid->XPos != 0 && DCGrid->YPos != 2) && DCGrid->XPos == Obstacle_XPosCount) // 기획안대로 X = 0 , Y = 2 에서는 장애물을 생성하지 않음
		{ 
			// 장애물이 한쪽 위치에 치우쳐지는걸 방지하기 위해서 보정치를 이용해서 지그재그로 생성
			if((Obstacle_XPosCount + XPosLocRandValue) % 2 == 0 && (DCGrid->YPos == Obstacle_YPosRand_First) && !(numberOfObstacle == MaxnumberOfObstacle))
			{	
				// 장애물 스폰, 장애물의 종류는 랜덤으로 생성, 장애물 종류에따라 위치와 스케일 보정치가 정해짐
				AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f), this->GetActorRotation(), SpawnParams);
				// 중앙 도로에서 생성시 장애물에 위치 보정치를 줘서 위치 초기화
				if (DCGrid->YPos == 1 || DCGrid->YPos == 3)
					SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + FVector(0.f, 0.f, -15.f));
				// 장애물의 위치, 스케일 보정치에 따라 조정
				SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + ObstacleLocCorrectionValue);
				SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
				// 장애물 배열에 스폰된 장애물을 추가
				SpawnObtacle.Add(SpawnedObstacleRef);
				// 장애물 정보를 초기화
				Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;					
				Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
				Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
				// 장애물 아래 그리드의 정보를 초기화
				DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
				DCGrid->SetGridInfo_Material_Init();
	
				numberOfObstacle++;
				Obstacle_XPosCount++;
				// Obstacle_YPosRand_First는 게임 시작시 0,1 중 하나로 초기화 됨
				if (Obstacle_YPosRand_First == 1)
					Obstacle_YPosRand_First = 0;
				else
					Obstacle_YPosRand_First++;

			}
			else if((Obstacle_XPosCount + XPosLocRandValue) % 2 == 1 && (DCGrid->YPos == 3 + Obstacle_YPosRand_Second) && !(numberOfObstacle == MaxnumberOfObstacle))
			{				
				AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f) , this->GetActorRotation(), SpawnParams);
				
				if (DCGrid->YPos == 1 || DCGrid->YPos == 3)
					SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + FVector(0.f, 0.f, -15.f));
				
				SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + ObstacleLocCorrectionValue);
				SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
				SpawnObtacle.Add(SpawnedObstacleRef);
				Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;

				Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
				Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
				DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
				DCGrid->SetGridInfo_Material_Init();

				numberOfObstacle++;
				Obstacle_XPosCount++;		

				if (Obstacle_YPosRand_Second == 1)
					Obstacle_YPosRand_Second = 0;
				else
					Obstacle_YPosRand_Second++;
			}
		}		
	}
	else if (false) // 중립 지역에는 장애물이 안나오도록 기획 변경
	{
		if (DCGrid->YPos != 2)
		{
			if (DCGrid->XPos == Obstacle_XPosCount_netural)
			{
				if (Obstacle_XPosCount_netural % 2 == 0 && (DCGrid->YPos == 0 || DCGrid->YPos == 1)) // xcout가 짝수 y = 0,1일때
				{
					if (FMath::RandRange(0, 100) <= 50.0f + CorrentionSpawnObstacle && !(numberOfObstacle == MaxnumberOfObstacle))
					{
						AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f) + ObstacleLocCorrectionValue, this->GetActorRotation(), SpawnParams);
						SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
						SpawnObtacle.Add(SpawnedObstacleRef);
						Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;

						Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
						Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
						DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
						DCGrid->SetGridInfo_Material_Init();

						CorrentionSpawnObstacle = 0.f;
						numberOfObstacle++;
						Obstacle_XPosCount_netural++;
					}
					else
						CorrentionSpawnObstacle = 50.f;

				}
				else if (Obstacle_XPosCount_netural % 2 == 1 && (DCGrid->YPos == 3 || DCGrid->YPos == 4))
				{
					if (FMath::RandRange(0, 100) <= 50.0f + CorrentionSpawnObstacle && !(numberOfObstacle == MaxnumberOfObstacle))
					{
						AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f) + ObstacleLocCorrectionValue, this->GetActorRotation(), SpawnParams);
						SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
						SpawnObtacle.Add(SpawnedObstacleRef);
						Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;

						Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
						Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
						DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
						DCGrid->SetGridInfo_Material_Init();

						CorrentionSpawnObstacle = 0.f;
						numberOfObstacle++;
						Obstacle_XPosCount_netural++;
					}
					else
						CorrentionSpawnObstacle = 50.f;
				}
			}
		}
	}
	else if (Pattern == EEventBoxState::EnemyArea)
	{
		if ((DCGrid->XPos != 14 && DCGrid->YPos != 2) && (DCGrid->XPos == Obstacle_XPosCount_Enemy))
		{
			if (Obstacle_XPosCount_Enemy % 2 == 0 && (DCGrid->YPos == Obstacle_YPosRand_First) && !(numberOfObstacle == MaxnumberOfObstacle))
			{
				AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f), this->GetActorRotation(), SpawnParams);
			
				if (DCGrid->YPos == 1 || DCGrid->YPos == 3)
					SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + FVector(0.f, 0.f, -15.f));
				
				SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + ObstacleLocCorrectionValue);
				SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
				SpawnObtacle.Add(SpawnedObstacleRef);
				
				Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;					
				Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
				Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
				
				DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
				DCGrid->SetGridInfo_Material_Init();

				numberOfObstacle++;
				Obstacle_XPosCount_Enemy++;
				
				if (Obstacle_YPosRand_First == 1)
					Obstacle_YPosRand_First = 0;
				else
					Obstacle_YPosRand_First++;
				
			}
			else if (Obstacle_XPosCount_Enemy % 2 == 1 && (DCGrid->YPos == 3 + Obstacle_YPosRand_Second) && !(numberOfObstacle == MaxnumberOfObstacle))
			{				
				AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f), this->GetActorRotation(), SpawnParams);
				
				if (DCGrid->YPos == 1 || DCGrid->YPos == 3)
					SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + FVector(0.f, 0.f, -15.f));
				
				SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + ObstacleLocCorrectionValue);
				SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
				SpawnObtacle.Add(SpawnedObstacleRef);		
				
				Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;
				Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
				Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
				
				DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
				DCGrid->SetGridInfo_Material_Init();

				numberOfObstacle++;
				Obstacle_XPosCount_Enemy++;
				
				if (Obstacle_YPosRand_Second == 1)
					Obstacle_YPosRand_Second = 0;
				else
					Obstacle_YPosRand_Second++;				
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

void AGridManager::Spawned_EventObject()
{
	FActorSpawnParameters SpawnParams;
					
	AActor* SpawndObjectRef = GetWorld()->SpawnActor<AActor>(ObjectToSpawn, parentBattleTrigger->Coordinate[ObjectToSpawnXPos].MultiArray[ObjectToSpawnYPos].pGrid->GetActorLocation() + FVector(ObjectToSpawnLocXPos, ObjectToSpawnLocYPos, ObjectToSpawnLocZPos), this->GetActorRotation(), SpawnParams);


}

TSubclassOf<AActor> AGridManager::GetRandObstacleInArray()
{
	TSubclassOf<AActor> GetActor;

	
	int32 RandValue = FMath::RandRange(1, ObstacleToSpawnArray.Num());
	GetActor = ObstacleToSpawnArray[RandValue - 1];

	switch ((RandValue))
	{
		case 1:
			ObstacleLocCorrectionValue = FVector(-10.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 2:
			ObstacleLocCorrectionValue = FVector(-10.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 3:
			ObstacleLocCorrectionValue = FVector(-10.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 4:
			ObstacleLocCorrectionValue = FVector(-10.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 5:
			ObstacleLocCorrectionValue = FVector(0.f, 0.f, 5.f);
			ObstacleScaleCorrectionValue = FVector(1.5f, 1.f, 1.f);
			break;
		case 6:
			ObstacleLocCorrectionValue = FVector(0.f, 0.f, 5.f);
			ObstacleScaleCorrectionValue = FVector(1.5f, 1.f, 1.f);
			break;
		case 7:
			ObstacleLocCorrectionValue = FVector(-70.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.5f);
			break;
		case 8:
			ObstacleLocCorrectionValue = FVector(-55.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.5f);
			break;
		case 9:
			ObstacleLocCorrectionValue = FVector(0.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.25f);
			break;
		case 10:
			ObstacleLocCorrectionValue = FVector(0.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 0.75f, 1.25f);
			break;
		case 11:
			ObstacleLocCorrectionValue = FVector(0.f, 0.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 12:
			ObstacleLocCorrectionValue = FVector(0.f, 50.f, 40.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 13:
			ObstacleLocCorrectionValue = FVector(0.f, -10.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.5f, 1.f);
			break;
		case 14:
			ObstacleLocCorrectionValue = FVector(-60.f, -60.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		case 15:
			ObstacleLocCorrectionValue = FVector(-60.f, -60.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.25f, 1.25f);
			break;
		case 16:
			ObstacleLocCorrectionValue = FVector(-60.f, 60.f, 10.f);
			ObstacleScaleCorrectionValue = FVector(1.f, 1.f, 1.f);
			break;
		default:
		break;
	}

	return GetActor;
}



void AGridManager::SetEnemyCharacterStatData(AEnemySquadCharacter* EnemyCharacter , int32 BrunchNum)
{
	// GameInsatnce에 저장된 적군 병과별 능력치 데이터 테이블을 참조하여 병과 번호에 맞는 데이터를 불러온다.
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto DT_BrunchNumber = FText::FromStringTable("/Game/DATATABLE/EnemyBrunchNameData.EnemyBrunchNameData", 
																				FString::Printf(TEXT("%d"), 
																								BrunchNum));
	auto DT = gameIns->GetEnemyBrunchData(BrunchNum);

	// 병과 능력치 데이터를 적군 캐릭터에 능력치에 할당한다.
	EnemyCharacter->EnemyCharacter_BrunchName = DT_BrunchNumber;
	EnemyCharacter->Evasion += DT->EvasionCorrectionValue;
	EnemyCharacter->Critical += DT->CriticalCorrectionValue;
	EnemyCharacter->Accuracy += DT->AccuracyCorrectionValue;
	EnemyCharacter->SetWeaponStat(DT->EquipWeapon);
	EnemyCharacter->SetSkeletalMeshPath(BrunchNum);
	
}

int32 AGridManager::SetEnemyStatRand()
{
	TMap<int32, float> FinalMap;
	TMap < int32, float > brunchProb;

	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameDifficultyValue = gameIns->GetGameDifficulty();

	// 게임 난이도에 따라 생성되는 적 병과 조절
	int32 TotalProb = 0;
	TArray<int32> BrunchNum = { 0  , 2 , 4 , 6 , 8 };
	TArray<float> Prob = { 40.f + (-4.f*gameDifficultyValue),
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue) };
	for (int32 i = 0; i < 5; i++) { 
		// 0훈련병 40%, 1소총병 15%, 2경찰병 15%, 3돌격병 15%, 4저격병 15%
		// 병과번호와, 확률을 Map형식의 brunchProb에 저장
		brunchProb.Add(BrunchNum[i], Prob[i]);
	}

	// 확률의 총량을 계산(TotalProb)
	for (auto& CalTotalProbPair : brunchProb) {
		TotalProb += CalTotalProbPair.Value;
	}
	// 병과 번호와 병과 확률을 퍼센티지로 변환시켜서 FinalMap에 저장
	for (auto& ToCastPair : brunchProb) {
		float MProb = ToCastPair.Value / TotalProb;

		FinalMap.Add(ToCastPair.Key, MProb);
	}

	// 랜덤으로 정해진 실수를 바탕으로 누적확률계산을 하여 최종적으로 선출된 병과번호를 반환시킨다
	float randPivot = FMath::RandRange(0.f, 1.f);
	double acc = 0;

	for (auto& fianlCalc : FinalMap) {
		acc += fianlCalc.Value;

		if (randPivot <= acc) {
			
			return fianlCalc.Key;
		}
	}

	return NULL;
}
