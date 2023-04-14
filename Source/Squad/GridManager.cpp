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

	// �׸��� ���� ����
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Depth; j++)
		{
			// �׸��� ����
			FVector Loc(this->GetActorLocation().X - i * Interval_X, this->GetActorLocation().Y + j * Interval_Y, this->GetActorLocation().Z - 50.f);
						
			AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(GridToSpawn, Loc, this->GetActorRotation(), SpawnParams);
			SpawnGrids.Add(SpawnedActorRef);
			Cast<AGrid>(SpawnedActorRef)->parentGridManager = this;
			
			// �׸��� ���� �ʱ�ȭ
			AGrid* SpawnGrid = Cast<AGrid>(SpawnedActorRef);
			SpawnGrid->XPos = i + (EventBoxNumber * 5);
			SpawnGrid->YPos = j;
			SpawnGrid->GridInfo.GOTO = EGridOntheObject::Normal;

			// ���� �迭(��ǥ)�� �׸��� ����
			YGridArray[j] = (SpawnGrid);
			if (j == 4) {
				XGridArray[i] = YGridArray;
			}

			// ����(�Ʊ�,�߸�,��) ���� ��ֹ� ���� 
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
			// �׸��� �ʱ�ȭ
			// �׸��尡 �ʱ�ȭ�Ǹ鼭 BattleTriiger�� ��ǥ�迭�� ������ �ʱ�ȭ	
			SpawnGrid->SetGridState();
			SpawnGrid->SetVisible();
			SpawnGrid->InitGrid();
		}
	}
	
	// �Ʊ� ���� �׸��� ���¸� ��ֹ� ���·� ����
	if (EBState == EEventBoxState::FrindlyArea) {
		Spawned_ObstacleCheck();
	}
	// ���� Battle Tigger�� �̺�Ʈ ��Ȳ�� �����̺�Ʈ�̸� ���� ���� 
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
			// �׸��� ����
			FVector Loc(this->GetActorLocation().X - i * Interval_X, this->GetActorLocation().Y + j * Interval_Y, this->GetActorLocation().Z - 50.f);

			AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(GridToSpawn, Loc, this->GetActorRotation(), SpawnParams);
			SpawnGrids.Add(SpawnedActorRef);
			Cast<AGrid>(SpawnedActorRef)->parentGridManager = this;

			// �׸��� ���� �ʱ�ȭ
			AGrid* SpawnGrid = Cast<AGrid>(SpawnedActorRef);
			SpawnGrid->XPos = i + (EventBoxNumber * 5);
			SpawnGrid->YPos = j;
			SpawnGrid->GridInfo.GOTO = EGridOntheObject::Normal;

			// ���� �迭(��ǥ)�� �׸��� ����
			YGridArray[j] = (SpawnGrid);
			if (j == 4) {
				XGridArray[i] = YGridArray;
			}

			// ����(�Ʊ�,�߸�,��) ���� ��ֹ� ���� 
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

			// �׸��� �ʱ�ȭ
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

	// BattleTrigger ��ǥ �迭���� ���� ������ �ش��ϴ� ��ǥ�� ��ֹ��� ��ġ�� ��ǥ�� �ӽù迭�� ����
	for (int32 XPos =10; XPos < parentBattleTrigger->Coordinate.Num(); XPos++)	{
		for (int32 YPos = 0; YPos < parentBattleTrigger->Coordinate[XPos].MultiArray.Num(); YPos++)	{
			AActor* tempActor = parentBattleTrigger->Coordinate[XPos].MultiArray[YPos].pGrid;

			if (Cast<AGrid>(tempActor)->GridInfo.GOTO == EGridOntheObject::Obstacle) {
				tempArray.Add(tempActor);
			}
		}
	}

	// ��ֹ��� ��ġ�� ��ǥ ����(x+1)�� �� ĳ���͸� ����, �� ĳ���� ���� �ʱ�ȭ
	for (int32 i = 0; i < tempArray.Num(); i++) {
		auto pXPos = Cast<AGrid>(tempArray[i])->GridInfo.XPos;
		auto pYPos = Cast<AGrid>(tempArray[i])->GridInfo.YPos;

		if (pXPos < 14) {
			if (Cast<AGrid>(parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid)->GridInfo.GOTO != EGridOntheObject::Obstacle) {
				auto temp = parentBattleTrigger->Coordinate[pXPos + 1].MultiArray[pYPos].pGrid;
				// ��ĳ���͸� �����ϰ� �� ĳ���� �迭�� �ּҰ� �߰�
				AActor* SpawndCharacterRef = GetWorld()->SpawnActor<AActor>(EnemyCharacterToSpawn,
																			temp->GetActorLocation(),
																			this->GetActorRotation(),
																			SpawnParams);
				parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
				Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(temp));
				// �� ĳ���� ����(����, �޽� ��) �ʱ�ȭ
				SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
				// ĳ���Ͱ� ��� �ִ� �׸��� ���� �ʱ�ȭ
				Cast<AGrid>(temp)->GridInfo.GOTO = EGridOntheObject::Enemy;
				Cast<AGrid>(temp)->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
				Cast<AGrid>(temp)->SetGridInfo_Material_Init();
				// �� ĳ���� �˻� ���� ����
				numberOfEnemyCharacter++;				
			}
		}
	}
	
	// ��ֹ��� ���ġ ������ �� ĳ���Ͱ� ������ ��ġ�� ����
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
						// ��ĳ���͸� �����ϰ� �� ĳ���� �迭�� �ּҰ� �߰�
						parentBattleTrigger->EnemyList.Add(SpawndCharacterRef);
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetBelongToBattleTrigger(parentBattleTrigger);
						Cast<AEnemySquadCharacter>(SpawndCharacterRef)->SetUnderGrid(Cast<AGrid>(CastGrid));
						// �� ĳ���� ����(����, �޽� ��) �ʱ�ȭ
						SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), SetEnemyStatRand());
						// ĳ���Ͱ� ��� �ִ� �׸��� ���� �ʱ�ȭ
						CastGrid->GridInfo.GOTO = EGridOntheObject::Enemy;
						CastGrid->GridInfo.GridOnTheCharacter = SpawndCharacterRef;
						CastGrid->SetGridInfo_Material_Init();
						// �� ĳ���� �˻� ���� ����
						numberOfEnemyCharacter++;
					}
				}
			}
		} while (numberOfEnemyCharacter == MaxnumberOfEnemyCharacter - 1); // �� ĳ���ͼ��� �ִ�ġ�� �ɶ����� ����
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
				SetEnemyCharacterStatData(Cast<AEnemySquadCharacter>(SpawndCharacterRef), 1); // ���� ����
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
		if((DCGrid->XPos != 0 && DCGrid->YPos != 2) && DCGrid->XPos == Obstacle_XPosCount) // ��ȹ�ȴ�� X = 0 , Y = 2 ������ ��ֹ��� �������� ����
		{ 
			// ��ֹ��� ���� ��ġ�� ġ�������°� �����ϱ� ���ؼ� ����ġ�� �̿��ؼ� ������׷� ����
			if((Obstacle_XPosCount + XPosLocRandValue) % 2 == 0 && (DCGrid->YPos == Obstacle_YPosRand_First) && !(numberOfObstacle == MaxnumberOfObstacle))
			{	
				// ��ֹ� ����, ��ֹ��� ������ �������� ����, ��ֹ� ���������� ��ġ�� ������ ����ġ�� ������
				AActor* SpawnedObstacleRef = GetWorld()->SpawnActor<AActor>(GetRandObstacleInArray(), Grid->GetActorLocation() + FVector(0.f, 0.f, -20.f), this->GetActorRotation(), SpawnParams);
				// �߾� ���ο��� ������ ��ֹ��� ��ġ ����ġ�� �༭ ��ġ �ʱ�ȭ
				if (DCGrid->YPos == 1 || DCGrid->YPos == 3)
					SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + FVector(0.f, 0.f, -15.f));
				// ��ֹ��� ��ġ, ������ ����ġ�� ���� ����
				SpawnedObstacleRef->SetActorLocation(SpawnedObstacleRef->GetActorLocation() + ObstacleLocCorrectionValue);
				SpawnedObstacleRef->SetActorScale3D(ObstacleScaleCorrectionValue);
				// ��ֹ� �迭�� ������ ��ֹ��� �߰�
				SpawnObtacle.Add(SpawnedObstacleRef);
				// ��ֹ� ������ �ʱ�ȭ
				Cast<AObstacle>(SpawnedObstacleRef)->ObstacleCollectionValue = ObstacleLocCorrectionValue;					
				Cast<AObstacle>(SpawnedObstacleRef)->XPos = Cast<AGrid>(Grid)->XPos;
				Cast<AObstacle>(SpawnedObstacleRef)->YPos = Cast<AGrid>(Grid)->YPos;
				// ��ֹ� �Ʒ� �׸����� ������ �ʱ�ȭ
				DCGrid->GridInfo.GOTO = EGridOntheObject::Obstacle;
				DCGrid->SetGridInfo_Material_Init();
	
				numberOfObstacle++;
				Obstacle_XPosCount++;
				// Obstacle_YPosRand_First�� ���� ���۽� 0,1 �� �ϳ��� �ʱ�ȭ ��
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
	else if (false) // �߸� �������� ��ֹ��� �ȳ������� ��ȹ ����
	{
		if (DCGrid->YPos != 2)
		{
			if (DCGrid->XPos == Obstacle_XPosCount_netural)
			{
				if (Obstacle_XPosCount_netural % 2 == 0 && (DCGrid->YPos == 0 || DCGrid->YPos == 1)) // xcout�� ¦�� y = 0,1�϶�
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
 
void AGridManager::Spawned_ObstacleCheck() // �Ʊ� ������
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
	// GameInsatnce�� ����� ���� ������ �ɷ�ġ ������ ���̺��� �����Ͽ� ���� ��ȣ�� �´� �����͸� �ҷ��´�.
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto DT_BrunchNumber = FText::FromStringTable("/Game/DATATABLE/EnemyBrunchNameData.EnemyBrunchNameData", 
																				FString::Printf(TEXT("%d"), 
																								BrunchNum));
	auto DT = gameIns->GetEnemyBrunchData(BrunchNum);

	// ���� �ɷ�ġ �����͸� ���� ĳ���Ϳ� �ɷ�ġ�� �Ҵ��Ѵ�.
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

	// ���� ���̵��� ���� �����Ǵ� �� ���� ����
	int32 TotalProb = 0;
	TArray<int32> BrunchNum = { 0  , 2 , 4 , 6 , 8 };
	TArray<float> Prob = { 40.f + (-4.f*gameDifficultyValue),
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue), 
							15.f + (1.f*gameDifficultyValue) };
	for (int32 i = 0; i < 5; i++) { 
		// 0�Ʒú� 40%, 1���Ѻ� 15%, 2������ 15%, 3���ݺ� 15%, 4���ݺ� 15%
		// ������ȣ��, Ȯ���� Map������ brunchProb�� ����
		brunchProb.Add(BrunchNum[i], Prob[i]);
	}

	// Ȯ���� �ѷ��� ���(TotalProb)
	for (auto& CalTotalProbPair : brunchProb) {
		TotalProb += CalTotalProbPair.Value;
	}
	// ���� ��ȣ�� ���� Ȯ���� �ۼ�Ƽ���� ��ȯ���Ѽ� FinalMap�� ����
	for (auto& ToCastPair : brunchProb) {
		float MProb = ToCastPair.Value / TotalProb;

		FinalMap.Add(ToCastPair.Key, MProb);
	}

	// �������� ������ �Ǽ��� �������� ����Ȯ������� �Ͽ� ���������� ����� ������ȣ�� ��ȯ��Ų��
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
