// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleController.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "SquadAIController.h"
#include "SquadGameMode.h"
#include "TimerManager.h"
#include "Grid.h"
#include "Engine/World.h"


// Sets default values
ABattleController::ABattleController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	BGMComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));

}

// Called when the game starts or when spawned
void ABattleController::BeginPlay()
{
	Super::BeginPlay();
		
	
	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->BCIns = this;


	BGMComp->SetSound(ExplorerSound);
	BGMComp->Play(3.f);
}

// Called every frame
void ABattleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

///////////////////////////// Init Part ////////////////////////////////////

void ABattleController::InitBattleSetting(TArray<AActor*> EnemyList, AActor* triggerBox)
{
	// triger
	if (pTriggerBox == nullptr)  pTriggerBox = Cast<ABattleTrigger>(triggerBox);

	ClearArray();

	GetFriendlyCharacters(); // SMC
	GetEnemyCharacters(EnemyList); // trigger

	SortCharacters(); // 

	SystemState.init();

	EnemyDeathCount = 0;
	
	if (AllCharacters.Num() > 0)
	{
		SystemState.CharNumber = AllCharacters.Num();
		SystemState.SelectedCharacter = nullptr;
	}


	StartTurnSystem_init();
}

void ABattleController::GetFriendlyCharacters()
{
	auto gameIns = GetWorld()->GetGameInstance();
	FriendlyCharacters.Append(Cast<USquadGameInstance>(gameIns)->SCMIns->FriendlyCharList);
	for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
	{
		Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->ArrayNumbering = i;
	}
	PlayerStartBattleArray.Append(FriendlyCharacters);
}

void ABattleController::GetEnemyCharacters(TArray<AActor*> EnemyList)
{	
	EnemyCharacters.Append(EnemyList);

	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->Fun_Death.BindUFunction(this, FName("AddEnemyDeathCount"));
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->ArrayNumbering = i;
		
	}
	//
	//UGameplayStatics::GetAllActorsOfClass(this, AEnemySquadCharacter::StaticClass(), EnemyCharacters);
}

void ABattleController::SortCharacters()
{
	AllCharacters.Append(FriendlyCharacters);
	AllCharacters.Append(EnemyCharacters);
}

void ABattleController::ClearArray()
{
	FriendlyCharacters.Empty();
	EnemyCharacters.Empty();
	AllCharacters.Empty();
	PlayerStartBattleArray.Empty();
	PlayerEndBattleArray.Empty();
	tempPlayerEndBattleArray.Empty();
	EnemyStartBattleArray.Empty();
	EnemyEndBattleArray.Empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ABattleController::StartTurnSystem_init() // 
{
	IsBattleStart = true;

	auto gameMode = UGameplayStatics::GetGameMode(this);
	
	// UI
	Cast<ASquadGameMode>(gameMode)->ViewBattleWidget();

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = nullptr;
	
	// 
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;
	Cast<USquadGameInstance>(gameIns)->SCMIns->zoomswitch();

	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(false);
	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();

	Cast<USquadGameInstance>(gameIns)->SCMIns->IsExploreToBattle = true;
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetBattleInit(Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->GetNeturalAreaLocation());
	Cast<USquadGameInstance>(gameIns)->SCMIns->MoveSwitch = true;

	Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->BattleTrigger_PlayerSpreadOut();

	// 
	
	EnemyStartBattleArray = (EnemyCharacters);

	//
		
	
	
	/*
	if (SystemState.HaveTurnCharacter == Cast<APlayerSquadCharacter>(SystemState.HaveTurnCharacter))
	{
		Cast<APlayerSquadCharacter>(SystemState.HaveTurnCharacter)->Debug_Shot();
	}
	*/
	//Cast<APlayerSquadCharacter>(FriendlyCharacters[0])->Debug_Shot();


	// 

	BGMComp->SetSound(BattleSound);
	BGMComp->FadeIn(3.f);

	StartTurnSystem();
}

void ABattleController::ControlCharacterCameraMovement(bool PlayerMovementSwitch)
{
	auto gameIns = GetWorld()->GetGameInstance();

	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(PlayerMovementSwitch);
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();
}

void ABattleController::StartTurnSystem()
{
	for (int32 i = 0; i < AllCharacters.Num(); i++)
	{
		Cast<ASquadCharacter>(AllCharacters[i])->StateEnum = EStateEnum::SE_End;
	}	
	//
	if (!WhosTurn) // 
	{
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}
	}
	else // 
	{
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}
	}
}

void ABattleController::SetSelectedCharacter(ASquadCharacter* SelectedChararcter)
{
	auto gameIns = GetWorld()->GetGameInstance();
	SystemState.SelectedCharacter = SelectedChararcter;
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;

	FString name = SystemState.SelectedCharacter->GetName();
	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeCurrentCharName(name);
}

void ABattleController::ClearSelectedCharacter()
{
	auto gameIns = GetWorld()->GetGameInstance();	
	SystemState.SelectedCharacter = nullptr;
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;

	FString name = TEXT("nullptr");
	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeCurrentCharName(name);
}

void ABattleController::SetEnd()
{
	if (SystemState.SelectedCharacter == Cast<AEnemySquadCharacter>(SystemState.SelectedCharacter) && SetDebug == true)
	{
		EndTurnSystem();
	}
}

void ABattleController::EndTurnSystem()
{
	BeCheck();
	UE_LOG(LogClass, Log, TEXT(" StartArray : %d "), PlayerStartBattleArray.Num());
	UE_LOG(LogClass, Log, TEXT(" EndArray : %d "), PlayerEndBattleArray.Num());
	if (!WhosTurn && IsBattleStart == true)
	{
		 // 적이 모두 죽었는지 안죽었는지 판별
		
			if (PlayerEndBattleArray.Num() == FriendlyCharacters.Num() - PlayerDeathCount) 
			{
				for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
				{
					if(Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum != EStateEnum::SE_Death)
					Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_End;					
				}

				for (int32 i = 0; i < EnemyCharacters.Num(); i++)
				{
					if (Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum != EStateEnum::SE_Death)
					Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
				}

				WhosTurn = true;
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
				
				if (EnemyEndBattleArray.Num() == EnemyCharacters.Num() - EnemyDeathCount)
				{
					EnemyStartBattleArray.Append(EnemyEndBattleArray); // 행동 끝낸 적 캐릭터들을 다시 행동력을 채우고
					for (int32 i = 0; i < EnemyStartBattleArray.Num(); i++) // 행동력을 채운 적 캐릭터 장판을 다시 초록색
					{
						if (Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->GetUnderGrid() != nullptr)
						{
							AGrid* tempGrid = Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->GetUnderGrid();
							tempGrid->SetGridInfo_Material_temp3();
						}
					}
					EnemyEndBattleArray.Empty();                       // EndArray를 비운다
				}


				if (IsBattleStart) // 마지막 적을 잡았을때 생길 수 있는 상황을 방지
				{ 
					EndTurnSystem_Enemy();	
					
				}

				
				
			}			
			
			//StartTurnSystem();
	}
}

void ABattleController::EndTurnSystem_Enemy()
{
	if (WhosTurn) 
	{
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				WorkEnemyAI();

			}), WaitTime, false);
		

	}

	//WhosTurn = false;
	//Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);


	// 발판 색깔 변환 - 임시 삭제 02/09

	/*

	*/
}

void ABattleController::BeCheck()
{
	if (EnemyDeathCount == EnemyCharacters.Num()) // 적의 수와 적의 죽은수가 같으면
	{
		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			if (Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum == EStateEnum::SE_Death)
			{
				//EnemyCharacters[i]->Destroy();
			}
		}
		auto gameMode = UGameplayStatics::GetGameMode(this);
	
	// UI
		ResultBattle_temp();
		Cast<ASquadGameMode>(gameMode)->ChangeMenuWidget(Cast<ASquadGameMode>(gameMode)->GetVictoryWidgetClass());
		
		//ResultBattle();

				
	}
	else if(PlayerDeathCount == FriendlyCharacters.Num()) // 플레이어 전멸
	{
		//Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		auto gameMode = UGameplayStatics::GetGameMode(this);

		// UI
		ResultBattle_temp();
		Cast<ASquadGameMode>(gameMode)->ChangeMenuWidget(Cast<ASquadGameMode>(gameMode)->GetDefeatWidgetClass());
		
		//ResultBattle();
	}
	// 같지 않다면 다음 턴으로
}

// 전투 리스트에서


void ABattleController::WorkEnemyAI()
{
	// 적전투종료리스트와 적리스트의 값이 같으면 종료
	//if (EnemyEndBattleArray.Num() == EnemyCharacters.Num() - EnemyDeathCount)
	//{
	//	EnemyStartBattleArray = (EnemyEndBattleArray); // 행동 끝낸 적 캐릭터들을 다시 행동력을 채우고
	//	EnemyEndBattleArray.Empty();                       // EndArray를 비운다
	//}
	if (PlayerDeathCount == FriendlyCharacters.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
		BeCheck();		
	}
	else if(EnemyEndBattleArray.Num() != EnemyCharacters.Num() - EnemyDeathCount) // 그렇지 않다면 전투리스트 인덱스 0번째의 적 캐릭터를 팝한다.
	{
		AEnemySquadCharacter* Enemy = Cast<AEnemySquadCharacter>(EnemyStartBattleArray[0]);
		EnemyStartBattleArray.RemoveAt(0);
		ASquadAIController* controller = Cast<ASquadAIController>(Enemy->GetController());		
		
	
		if(PlayerStartBattleArray.Num() == 0)
		controller->EnemyChararacter_SetFrindlyCharacterList(PlayerEndBattleArray);
		else
		controller->EnemyChararacter_SetFrindlyCharacterList(PlayerStartBattleArray);

		controller->EnemyCharacter_ActiveAI();
		if(Enemy->GetUnderGrid() != nullptr)
		Enemy->GetUnderGrid()->SetGridInfo_Material_temp2(); // 장판 검정색으로 변환
		EnemyEndBattleArray.Push(Enemy);

		// 전투를 실행시킨다
		// 전투가 끝나면 전투종료리스트에 푸쉬한다.
		//if(EnemyStartBattleArray.Num() != 0)
		{
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					WorkEnemyAI();

				}), WaitTime, false);
		}

	}
	else if(EnemyEndBattleArray.Num() == EnemyCharacters.Num() - EnemyDeathCount)
	{
		ResetPlayerEndBattleArray();
		for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		{
			AGrid* tempGrid = Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->GetUnderGrid();
			tempGrid->SetGridInfo_Material_temp3();
		}

		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_End;
		}

		for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		{
			Cast<ASquadCharacter>(PlayerStartBattleArray[i])->StateEnum = EStateEnum::SE_Stay;
			Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->Buff_System();
			Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->StopMontage();
		}

		WhosTurn = false;
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		
	}
}

// Enemy Start & End Battle Array

void ABattleController::RemoveFromEnemyEndBattleArray(int32 ArrayNumbering)
{
	if(EnemyEndBattleArray.Num() > 0)
	{ 
		EnemyEndBattleArray.RemoveAt(ArrayNumbering);
		for(int32 i = 0 ; i < EnemyEndBattleArray.Num() ; i++)
		Cast<AEnemySquadCharacter>(EnemyEndBattleArray[i])->ArrayNumbering = i;
	}
	else if (EnemyStartBattleArray.Num() > 0) // 첫턴에 원턴킬 날때 생기는 오류 보정 - 만약 적이 그럴 수 있는 상황이 생기면 아군 배열에도 추가해야함
	{
		EnemyStartBattleArray.RemoveAt(ArrayNumbering);
		for (int32 i = 0; i < EnemyStartBattleArray.Num(); i++)
			Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->ArrayNumbering = i;
	}
}

void ABattleController::RemoveFromPlayerEndBattleArray(int32 ArrayNumbering, int32 Numbering)
{
	PlayerEndBattleArray.RemoveAt(ArrayNumbering);
	for (int32 i = 0; i < PlayerEndBattleArray.Num(); i++)
		Cast<APlayerSquadCharacter>(PlayerEndBattleArray[i])->ArrayNumbering = i;
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	Cast<USquadGameInstance>(gameIns)->SCMIns->FriendlyCharList.RemoveAt(Numbering);
	for (int32 i = 0; i < Cast<USquadGameInstance>(gameIns)->SCMIns->FriendlyCharList.Num(); i++)
		Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(gameIns)->SCMIns->FriendlyCharList[i])->numbering = i;
	PlayerDeathCount++;
}

void ABattleController::AddPlayerEndBattleArray(AActor* Actor)
{
	UE_LOG(LogClass, Log, TEXT("Start: %d  End : %d"), PlayerStartBattleArray.Num(), PlayerEndBattleArray.Num());
	auto tempPlayerChar = Cast<APlayerSquadCharacter>(Actor);
	PlayerStartBattleArray.RemoveAt(tempPlayerChar->ArrayNumbering);
	for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->ArrayNumbering = i;
	PlayerEndBattleArray.Add(tempPlayerChar);
	tempPlayerChar->ArrayNumbering = PlayerEndBattleArray.Num() - 1;
	UE_LOG(LogClass, Log, TEXT("Start: %d  End : %d"), PlayerStartBattleArray.Num(), PlayerEndBattleArray.Num());
}

void ABattleController::ResetPlayerEndBattleArray()
{
	if (PlayerEndBattleArray.Num() == FriendlyCharacters.Num() - PlayerDeathCount)
	{
		PlayerStartBattleArray.Append(PlayerEndBattleArray); // 행동 끝낸 아군 캐릭터들을 다시 행동력을 채우고
		//if(PlayerEndBattleArray.Num() != 0)
		PlayerEndBattleArray.Empty();              // EndArray를 비운다

		UE_LOG(LogClass, Log, TEXT("Start: %d  End : %d"), PlayerStartBattleArray.Num(), PlayerEndBattleArray.Num());
	}
}

void ABattleController::AddEnemyDeathCount()
{
	EnemyDeathCount++;
}

void ABattleController::ResultBattle()
{
	// BeCheck()
	auto gameMode = UGameplayStatics::GetGameMode(this);

	//

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SCMIns->zoomswitch();

	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(true);
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();

	Cast<USquadGameInstance>(gameIns)->SCMIns->IsBattleToExplore = true;
	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetBattleInit(Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->GetNeturalAreaLocation()); // 이 코드 수정해야할 필요가 있음
	Cast<USquadGameInstance>(gameIns)->SCMIns->MoveSwitch = true;
	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetBattleEnd();
	
	//


	//Cast<ASquadGameMode>(gameMode)->EndBattle();

	BGMComp->FadeOut(3.f , 0.f);
	BGMComp->SetSound(ExplorerSound);
	BGMComp->FadeIn(2.f);

	// 
}

void ABattleController::ResultBattle_temp()
{
	ClearArray();

	IsBattleStart = false;

	//
	pTriggerBox->DeleteBattleTrigger();
	pTriggerBox = nullptr;

	EnemyDeathCount = 0;
	PlayerDeathCount = 0;
}

