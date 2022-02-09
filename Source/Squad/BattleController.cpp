// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleController.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "SquadAIController.h"
#include "SquadGameMode.h"

// Sets default values
ABattleController::ABattleController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	

}

// Called when the game starts or when spawned
void ABattleController::BeginPlay()
{
	Super::BeginPlay();
		
	
	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->BCIns = this;
	
	
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


	StartTurnSystem();
}

void ABattleController::GetFriendlyCharacters()
{
	auto gameIns = GetWorld()->GetGameInstance();
	FriendlyCharacters.Append(Cast<USquadGameInstance>(gameIns)->SCMIns->FriendlyCharList);

}

void ABattleController::GetEnemyCharacters(TArray<AActor*> EnemyList)
{	
	EnemyCharacters.Append(EnemyList);

	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->Fun_Death.BindUFunction(this, FName("AddEnemyDeathCount"));
		//AddEnemyDeathCount();
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ABattleController::StartTurnSystem() // 
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
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(false);
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetBattleInit(Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->GetNeturalAreaLocation());

	Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->Debug_Fuc();

	// 
		
	for (int32 i = 0; i < AllCharacters.Num(); i++)
	{
		if(Cast<AEnemySquadCharacter>(AllCharacters[i]) == AllCharacters[i]) 
		{	
			Cast<AEnemySquadCharacter>(AllCharacters[i])->StateEnum = EStateEnum::SE_End;		
		}
		else
		{			
			Cast<ASquadCharacter>(AllCharacters[i])->StateEnum = EStateEnum::SE_End;
		}
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
	
	/*
	if (SystemState.HaveTurnCharacter == Cast<APlayerSquadCharacter>(SystemState.HaveTurnCharacter))
	{
		Cast<APlayerSquadCharacter>(SystemState.HaveTurnCharacter)->Debug_Shot();
	}
	*/
	//Cast<APlayerSquadCharacter>(FriendlyCharacters[0])->Debug_Shot();


	// 
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
	if (!WhosTurn)
	{
		BeCheck();

		int32 TurnCont = 0;
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			
			if(Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum == EStateEnum::SE_End) 
			{
				TurnCont++;			
			}

			if (TurnCont == FriendlyCharacters.Num()) 
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

				if(IsBattleStart)
				EndTurnSystem_Enemy();
			}
		}

		
			
		
				
	}
	// �ӽ�	
	/*
	SystemState.TurnNumber++;

	if (SystemState.TurnNumber == SystemState.CharNumber)
	{
		SystemState.TurnNumber = 0;
	}

	SystemState.SelectedCharacter = AllCharacters[SystemState.TurnNumber];
	StartTurnSystem();

	// ���� ������ ������ ������
	*/
}

void ABattleController::EndTurnSystem_Enemy()
{
	if (WhosTurn) 
	{
		//WorkEnemyAI();

		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_End;
		}
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}

		WhosTurn = false;
	}

	// 발판 색깔 변환 - 임시 삭제 02/09

	/*
	for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
	{
		Cast<ASquadCharacter>(FriendlyCharacters[i])->pGridOnCharacter->SetGridInfo_Material_temp3();
	}
	*/
}

void ABattleController::BeCheck()
{
	// �� ���������� üũ
	// bool CheckTurn = false; // �� üũ ���� false �Ʊ� // true ����
	if (EnemyDeathCount == EnemyCharacters.Num())
	{
		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			if (Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum == EStateEnum::SE_Death)
			{
				//EnemyCharacters[i]->Destroy();
			}
		}
		
		ResultBattle();
	}
	else
	{
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
	}

	
	/*
	if (1) // �Ʊ��� ���϶�
	{
		for (int32 i = 0; FriendlyCharacters.Num(); i++)
		{
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}
	}
	else // ������ ���� ��
	{
		if (EnemyDeathCount == EnemyCharacters.Num())
		{
			ResultBattle();
		}
	}
	*/



	// ���� ������
	// IsBattleStart = false;
	// ClearArray();

}

void ABattleController::WorkEnemyAI()
{
	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		if(Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum != EStateEnum::SE_Death)
		{
			ASquadAIController* controller = Cast<ASquadAIController>(EnemyCharacters[i]->GetInstigatorController());
			//controller->ActiveAI();
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_End;
		}
	}
}

void ABattleController::AddEnemyDeathCount()
{
	EnemyDeathCount++;
}

void ABattleController::ResultBattle()
{
	// BeCheck()���� ������ ���������� �Լ��� �߰��ؾ���
	ClearArray();

	IsBattleStart = false;
	
	//
	pTriggerBox->DeleteBattleTrigger();
	pTriggerBox = nullptr;
	
	//

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SCMIns->zoomswitch();
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(true);
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();

	// ���� ���� �Լ�

	auto gameMode = UGameplayStatics::GetGameMode(this);

	Cast<ASquadGameMode>(gameMode)->EndBattle();

	// ĳ���� ���� �Լ� ��� �ʿ�
}

