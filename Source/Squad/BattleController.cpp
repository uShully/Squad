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
#include "BattleWidget.h"
#include "SquadController.h"
#include "SquadCharacterStatComponent.h"
#include "CharacterAnimInstance.h"
#include "Engine/World.h"
#include "ExploreWidget.h"
#include "BattleTrigger.h"


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
		//SystemState.SelectedCharacter = nullptr;
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
		//Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetStatustBarWidget()->SetRenderOpacity(0.5f);
	}
	PlayerStartBattleArray.Append(FriendlyCharacters);
}

void ABattleController::GetEnemyCharacters(TArray<AActor*> EnemyList)
{	
	EnemyCharacters.Append(EnemyList);

	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		//Cast<AEnemySquadCharacter>(EnemyCharacters[i])->Fun_Death.BindUFunction(this, FName("AddEnemyDeathCount"));
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->ArrayNumbering = i;
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
		// [BUG] 버그 발생 22/11/09 95번째 줄
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


void ABattleController::StartTurnSystem_init() 
{
	IsBattleStart = true;

	auto gameMode = UGameplayStatics::GetGameMode(this);
	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
	// UI
	//Cast<UExploreWidget>(Cast<ASquadGameMode>(gameMode)->GetExploreWidget())->BeHideTaskCanvas();
	Cast<ASquadGameMode>(gameMode)->ViewBattleWidget();
	Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = nullptr;
	Cast<USquadGameInstance>(gameIns)->IsBattleStart = true;
	
	// controller

	Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->ClearSelectedCharacter_Explorer();


	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;


	Cast<USquadGameInstance>(gameIns)->SCMIns->zoomswitch();
	Cast<USquadGameInstance>(gameIns)->SCMIns->IsExploreToBattle = true;
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetExploreInit(Cast<USquadGameInstance>(gameIns)->SCMIns->GetActorLocation());
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_SetBattleInit(Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->GetNeturalAreaLocation());
	Cast<USquadGameInstance>(gameIns)->SCMIns->MoveSwitch = true;


	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);

	Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->BattleTrigger_PlayerSpreadOut();
	
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			//WorkEnemyAI();


			StartTurnSystem();

		}), 1.5f, false);
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

	
}

void ABattleController::ControlCharacterCameraMovement(bool PlayerMovementSwitch)
{	// true 면 움직임 false면 안움직임 
	// 카메라 변수는 false , 캐릭터 변수는 true일때 움직임
	auto gameIns = GetWorld()->GetGameInstance();
		
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(PlayerMovementSwitch);
	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement(!PlayerMovementSwitch);
}

bool ABattleController::SwitchCharacterMovement(bool PlayerMovementSwitch)
{
	auto gameIns = GetWorld()->GetGameInstance();

	Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(PlayerMovementSwitch);
	
	return Cast<USquadGameInstance>(gameIns)->SCMIns->ControlValue_CameraMovement;
}

void ABattleController::StartTurnSystem()
{
	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
	SplayerController->SetSquadControllerInput(true);
	EnableInput(SplayerController);

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

	for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
	{
		auto anim = Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->animInstance;
		Cast<UCharacterAnimInstance>(anim)->Set_IsBattle(true);
		Cast<UCharacterAnimInstance>(anim)->Call_GetIsBattle();
	}

	auto bm = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	Cast<UBattleWidget>(bm->GetCurrentWidget())->ClearWidget_SkillPart();
	//SetSelectedCharacter(Cast<ASquadCharacter>(FriendlyCharacters[0]));

	APlayerSquadCharacter* SC = Cast<APlayerSquadCharacter>(GetSelectedCharacter());
	SetSelectedCharacter(Cast<ASquadCharacter>(SC));

	Cast<APlayerSquadCharacter>(SC)->SetHighLight(true);
	Cast<APlayerSquadCharacter>(SC)->SetGridOn();

	Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->IsGridSelected = true;
	Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(SC);
	Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(SC);

	bm->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(SC));
	bm->UpDateWidgetText(Cast<APlayerSquadCharacter>(SC));
	SC->PlaySelectedSound();


}

void ABattleController::SetSelectedCharacter(ASquadCharacter* SelectedChararcter)
{
	auto gameIns = GetWorld()->GetGameInstance();

	if (SelectedChararcter == nullptr) {
		SystemState.SelectedCharacter = nullptr;
		Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;
	}
	else {
		SystemState.SelectedCharacter = SelectedChararcter;
		Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;
			
		APlayerSquadCharacter* pSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeSkillButtonWidgetImage(pSelectedCharacter);
	}	
}

void ABattleController::ClearSelectedCharacter()
{
	auto gameIns = GetWorld()->GetGameInstance();	
	SystemState.SelectedCharacter = nullptr;
	Cast<USquadGameInstance>(gameIns)->SelectedCharacter = SystemState.SelectedCharacter;

	FString name = TEXT("nullptr");
	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeCurrentCharName(name);
}

AActor* ABattleController::GetSelectedCharacter()
{
	if (SystemState.SelectedCharacter != nullptr)
		return SystemState.SelectedCharacter;
	else
		return nullptr;
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
	//UE_LOG(LogClass, Log, TEXT(" StartArray : %d "), PlayerStartBattleArray.Num());
	//UE_LOG(LogClass, Log, TEXT(" EndArray : %d "), PlayerEndBattleArray.Num());
	if (!WhosTurn && IsBattleStart == true)
	{
		if (Cast<ASquadCharacter>(SystemState.SelectedCharacter)->IsGridOn) {	
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOff();
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(false);
		}

		auto gameMode = UGameplayStatics::GetGameMode(this);
						
		SetDisableSkillTargeting(true);
		SetDisableSkillTargeting(false);

		Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		SystemChangeSelectedCharacter();
		if(SystemState.SelectedCharacter != nullptr) {
			Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		}
		// 적이 모두 죽었는지 안죽었는지 판별
		
			if (PlayerEndBattleArray.Num() == FriendlyCharacters.Num() - PlayerDeathCount) 
			{
				for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
				{
					if(Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum != EStateEnum::SE_Death)
					Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_End;			
					Update_PlayerCharacterCooldown(Cast<APlayerSquadCharacter>(FriendlyCharacters[i])); // 살아있는 캐릭터 쿨다운 계산
					
					Cast<ASquadCharacter>(FriendlyCharacters[i])->Control_CCArray();		// 스킬 효과 가감 함수 추가 예정 12/22
					Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->SetHighLight(false);
				}

				for (int32 i = 0; i < EnemyCharacters.Num(); i++)
				{
					if (Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum != EStateEnum::SE_Death)
					Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
				}

				WhosTurn = true;
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetSquadControllerInput(false);
				
				if (EnemyEndBattleArray.Num() == EnemyCharacters.Num() - EnemyDeathCount)
				{
					EnemyStartBattleArray.Append(EnemyEndBattleArray); // 행동 끝낸 적 캐릭터들을 다시 행동력을 채우고
					for (int32 i = 0; i < EnemyStartBattleArray.Num(); i++) // 행동력을 채운 적 캐릭터 장판을 다시 초록색
					{
						if (Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->GetUnderGrid() != nullptr)
						{
							AGrid* tempGrid = Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->GetUnderGrid();
							Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
							tempGrid->SetGridInfo_Material_Green();
						}
					}
					EnemyEndBattleArray.Empty();                       // EndArray를 비운다
				}


				if (IsBattleStart) // 마지막 적을 잡았을때 생길 수 있는 상황을 방지
				{ 
					Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
					EndTurnSystem_Enemy();	
					
				}

				
				
			}			
			
			//StartTurnSystem();
	}
}

void ABattleController::EndTurnSystem_Enemy()
{
	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);
	

	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		Cast<ASquadCharacter>(EnemyCharacters[i])->Control_CCArray();			// 스킬 효과 가감 함수 추가 예정 12/22
	}

	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->Set_BattleWidgetOpacity(0.5f); // 위젯 투명도

	if (WhosTurn) 
	{
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				WorkEnemyAI();

			}), WaitTime, false);
		

	}
}

void ABattleController::BeCheck()
{
	if (EnemyDeathCount == EnemyCharacters.Num()) // 적의 수와 적의 죽은수가 같으면
	{
		auto gameMode = UGameplayStatics::GetGameMode(this);
	
	// UI
		if (Cast<ABattleTrigger>(Cast<ASquadGameMode>(gameMode)->BTIns)->BTState == EBattleTriggerState::Boss)
		{
			BGMComp->Stop();
			Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ViewGameVictoryWidget();
		}
		else {
			ResultBattle_temp();

		
			//Cast<ASquadGameMode>(gameMode)->ChangeMenuWidget(Cast<ASquadGameMode>(gameMode)->GetVictoryWidgetClass());
			//BGMComp->Stop();
			Cast<ASquadGameMode>(gameMode)->ChangeSubWidget(Cast<ASquadGameMode>(gameMode)->GetVictoryWidgetClass());
			Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText_Right();
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->Target_Explorer = Cast<APlayerSquadCharacter>(GetSelectedCharacter());
		}
		//ResultBattle();

				
	}
	else if(PlayerDeathCount == FriendlyCharacters.Num()) // 플레이어 전멸
	{
		//Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		auto gameMode = UGameplayStatics::GetGameMode(this);

		// UI
		ResultBattle_temp();
		BGMComp->Stop();
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
	int32 stunCharactersNum = 0;

	for (int32 num = 0; num < EnemyStartBattleArray.Num(); num++) {
		if (Cast<AEnemySquadCharacter>(EnemyStartBattleArray[num])->IsStun) {
			stunCharactersNum++;
		}
	}

	if (PlayerDeathCount == FriendlyCharacters.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
		BeCheck();		
	}
	else if(EnemyEndBattleArray.Num() != EnemyCharacters.Num() - (EnemyDeathCount + stunCharactersNum)) // 그렇지 않다면 전투리스트 인덱스 0번째의 적 캐릭터를 팝한다.
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
		Enemy->GetUnderGrid()->SetGridInfo_Material_Black(); // 장판 검정색으로 변환
		EnemyEndBattleArray.Push(Enemy);

		// 전투를 실행시킨다
		// 전투가 끝나면 전투종료리스트에 푸쉬한다.
		//if(EnemyStartBattleArray.Num() != 0)
		/*
		{
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					WorkEnemyAI();

				}), WaitTime, false);
		}
		*/

	}
	else if(EnemyEndBattleArray.Num() == EnemyCharacters.Num() - (EnemyDeathCount + stunCharactersNum))
	{
		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(EnemyCharacters[i])->Control_CCArray();			// 스킬 효과 가감 함수 추가 예정 12/22
		}

		ResetPlayerEndBattleArray();
		for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		{
			AGrid* tempGrid = Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->GetUnderGrid();
			Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
			tempGrid->SetGridInfo_Material_Green();
		}

		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			if(Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum != EStateEnum::SE_Death)
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_End;
		}

		for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		{
			Cast<ASquadCharacter>(PlayerStartBattleArray[i])->StateEnum = EStateEnum::SE_Stay;
			Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->Buff_System();
			Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->StopMontage();
		}

		auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
		SplayerController->SetSquadControllerInput(true);
		EnableInput(SplayerController);

		WhosTurn = false;
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeWhosTurnName(WhosTurn);
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->Set_BattleWidgetOpacity(1.f);
		SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));

		auto gameMode = GetWorld()->GetAuthGameMode();
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			if(Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum != EStateEnum::SE_Death)
			Cast<ASquadCharacter>(FriendlyCharacters[i])->Control_CCArray();		// 스킬 효과 가감 함수 추가 예정 12/22
		}
		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter) {
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();

			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		}
		Cast<APlayerSquadCharacter>(PlayerStartBattleArray[0])->PlaySelectedSound();

		if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
			Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();

		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetSquadControllerInput(true);
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

	



	// UI
	//Cast<UExploreWidget>(Cast<ASquadGameMode>(gameMode)->GetExploreWidget())->BeHideTaskCanvas();
	//

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SCMIns->zoomswitch();

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
	if(IsBattleStart == true) { 
		for (int i = 0; i < FriendlyCharacters.Num(); i++) {
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->BeReload_BattleOver();
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
		}

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);

		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			auto anim = Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->animInstance;
			Cast<UCharacterAnimInstance>(anim)->Set_IsBattle(false);
			Cast<UCharacterAnimInstance>(anim)->Call_GetIsBattle();
		}
		auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
		//Cast<USquadGameInstance>(gameIns)->SCMIns->ControlValue_PlayerCharacterMovement_BeginPlay = true;

		for (int32 i = 0; i < FriendlyCharacters.Num(); i++) {
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetCharacterMovement()->MaxWalkSpeed = 200.f;
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetCharacterMovement()->MaxAcceleration = 150.f;
		}

		gameIns->SCMIns->SetUnitPos_Last_Location(gameIns->SCMIns->FriendlyCharList.Num());

		ClearArray();

		IsBattleStart = false;
		Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart = false;

		int32 LastUnit_Loc = gameIns->SCMIns->FriendlyCharList.Num();
		//gameIns->SCMIns->UnitPos_First->SetWorldLocation(Cast<ABattleTrigger>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->BTIns)->Coordinate[3].MultiArray[2].pGrid->GetActorLocation());
		//gameIns->SCMIns->UnitPos_Last->SetWorldLocation(Cast<ABattleTrigger>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->BTIns)->Coordinate[4-LastUnit_Loc].MultiArray[2].pGrid->GetActorLocation());
		Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->BTIns = nullptr;

		pTriggerBox->DeleteBattleTrigger();
		pTriggerBox = nullptr;

		EnemyDeathCount = 0;
		PlayerDeathCount = 0;
	}
}

void ABattleController::ChangeSelectedCharacter()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
		
	if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->StateEnum == EStateEnum::SE_End) {
		auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
		preSelectedCharacter->SetHighLight(false);
		
		if(Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering == FriendlyCharacters.Num() - 1)
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[0]));
		else
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering + 1]));
				
		SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->PlaySelectedSound();

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
	
	}
	else {
		if(PlayerStartBattleArray.Num() > 1) {
			if (PlayerStartBattleArray.Num() - 1 == Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering) {
				auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
				preSelectedCharacter->SetHighLight(false);

				SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
				Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			
				AddPlayerEndBattleArray(preSelectedCharacter);			
			}
			else {
				auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
				preSelectedCharacter->SetHighLight(false);			
			
				SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
				Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
				Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();

				AddPlayerEndBattleArray(preSelectedCharacter);			
			}

			Cast<APlayerSquadCharacter>(GetSelectedCharacter())->PlaySelectedSound();
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
		}
		else {
			AddPlayerEndBattleArray(SystemState.SelectedCharacter);
			SetSelectedCharacter(nullptr);
		}	
	}
}

void ABattleController::SystemChangeSelectedCharacter()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto gameMode = UGameplayStatics::GetGameMode(this);

	if (PlayerStartBattleArray.Num() > 1) {
		if (PlayerStartBattleArray.Num() - 1 == Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering) {
			auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
			preSelectedCharacter->SetHighLight(false);

			SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			AddPlayerEndBattleArray(preSelectedCharacter);
		}
		else {
			auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
			preSelectedCharacter->SetHighLight(false);
			
			SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();

			AddPlayerEndBattleArray(preSelectedCharacter);
		}

			Cast<APlayerSquadCharacter>(GetSelectedCharacter())->PlaySelectedSound();
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);

			if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
				Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();

			Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
			Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	}
	else {
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetHighLight(false);
		AddPlayerEndBattleArray(SystemState.SelectedCharacter);
		SetSelectedCharacter(nullptr);
	}
}


void ABattleController::ReverseControlChangeSelectedCharacter()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto gameMode = UGameplayStatics::GetGameMode(this);

	auto preSelectedCharacter = SystemState.SelectedCharacter;
	int32 tempValue;

	if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_End) {
		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering == FriendlyCharacters.Num() - 1) {

			int32 num = 0;
			while (1) {
				if (Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_End || Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_Death)
					++num;
				else
					break;
			}
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[num]));
		}
		else {
			int32 num = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering + 1;

			while (1)
			{
				if (Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_End || Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_Death)
					++num;
				else
					break;

				if (num == FriendlyCharacters.Num())
					num = 0;
			}
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[num]));
		}

		Cast<ASquadCharacter>(preSelectedCharacter)->SetGridOff();
		if (Cast<APlayerSquadCharacter>(preSelectedCharacter) == preSelectedCharacter) {
			Cast<APlayerSquadCharacter>(preSelectedCharacter)->SetHighLight(false);
		}

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);


		if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
			Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	}
	else {
		for (int i = 0; i < PlayerStartBattleArray.Num(); i++)	{
			if (Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i]) == Cast<APlayerSquadCharacter>(preSelectedCharacter)) {
				tempValue = i + 1;
			}
		}

		if (tempValue == PlayerStartBattleArray.Num())
			tempValue = 0;



		SetSelectedCharacter(Cast<APlayerSquadCharacter>(PlayerStartBattleArray[tempValue]));

		Cast<ASquadCharacter>(preSelectedCharacter)->SetGridOff();
		//Cast<ASquadCharacter>(preSelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
		if (Cast<APlayerSquadCharacter>(preSelectedCharacter) == preSelectedCharacter) {
			Cast<APlayerSquadCharacter>(preSelectedCharacter)->SetHighLight(false);
			if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum != EStateEnum::SE_Death && Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum != EStateEnum::SE_End)
			{
				if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Reload || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Cover)
				{
					SetDisableSkillTargeting(false);
					Cast<APlayerSquadCharacter>(preSelectedCharacter)->GetUnderGrid()->SetGridInfo_Material_Green();
				}
				else if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Shot || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Skill1 || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Skill2)
				{
					SetDisableSkillTargeting(true);
				}
				Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum = EStateEnum::SE_Stay;
			}
		}

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
	
		if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
			Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	}
}

void ABattleController::ControlChangeSelectedCharacter()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto gameMode = UGameplayStatics::GetGameMode(this);
	int32 tempValue;

	auto preSelectedCharacter = SystemState.SelectedCharacter;

	if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_End) {


		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering == 0) {

			int32 num = 3;
			while (1) {
				if (Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_End || Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_Death)
					--num;
				else
					break;
			}
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[num]));
		}
		else {
			int32 num = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering - 1;

			while (1)
			{
				if (Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_End || Cast<APlayerSquadCharacter>(FriendlyCharacters[num])->StateEnum == EStateEnum::SE_Death)
					--num;
				else
					break;

				if (num == -1)
					num = 3;
			}
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[num]));
		}

		Cast<ASquadCharacter>(preSelectedCharacter)->SetGridOff();

		if (Cast<APlayerSquadCharacter>(preSelectedCharacter) == preSelectedCharacter) {
			Cast<APlayerSquadCharacter>(preSelectedCharacter)->SetHighLight(false);
		}

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);


		if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
			Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();

		
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();

	}
	else {
		for (int i = 0; i < PlayerStartBattleArray.Num(); i++) {
			if (Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i]) == Cast<APlayerSquadCharacter>(preSelectedCharacter)) {
				tempValue = i - 1;
			}
		}

		if (tempValue == -1)
			tempValue = PlayerStartBattleArray.Num() - 1;

		SetSelectedCharacter(Cast<APlayerSquadCharacter>(PlayerStartBattleArray[tempValue]));

		Cast<ASquadCharacter>(preSelectedCharacter)->SetGridOff();
		//Cast<ASquadCharacter>(preSelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
		if (Cast<APlayerSquadCharacter>(preSelectedCharacter) == preSelectedCharacter) {
			Cast<APlayerSquadCharacter>(preSelectedCharacter)->SetHighLight(false);
			if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum != EStateEnum::SE_Death && Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum != EStateEnum::SE_End)
			{
				if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Reload || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Cover)
				{
					SetDisableSkillTargeting(false);
					Cast<APlayerSquadCharacter>(preSelectedCharacter)->GetUnderGrid()->SetGridInfo_Material_Green();
				}
				else if (Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Shot || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Skill1 || Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum == EStateEnum::SE_Skill2)
				{
					SetDisableSkillTargeting(true);
				}
				Cast<APlayerSquadCharacter>(preSelectedCharacter)->StateEnum = EStateEnum::SE_Stay;
			}
		}

		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
		Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadGameMode>(gameMode)->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);

		if (Cast<ASquadGameMode>(gameMode)->GetCurrentWidget() == Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget()))
			Cast<UBattleWidget>(Cast<ASquadGameMode>(gameMode)->GetCurrentWidget())->ClearWidget_SkillPart();
				
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	}
}

void ABattleController::Update_PlayerCharacterCooldown(APlayerSquadCharacter* playerCharacter)
{
	if (playerCharacter->Character_Skill1_Cooldown > 0) {
		playerCharacter->Character_Skill1_Cooldown--;
	}
	if (playerCharacter->Character_Skill2_Cooldown > 0) {
		playerCharacter->Character_Skill2_Cooldown--;
	}
}

void ABattleController::SetSkillTargeting(bool IsAttackSkill)
{
	if (IsAttackSkill) {
		for (int32 i = 0; i < EnemyCharacters.Num(); i++) {
		//Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetHighLight(true);
			if(Cast<AEnemySquadCharacter>(EnemyCharacters[i])->StateEnum != EStateEnum::SE_Death) {
				Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetGridOn();
				Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetGridColor(FColor::Red);
			}
	}
	}
	else {
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetHighLight(true);
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetGridColor(FColor::Blue);
	}

}

void ABattleController::SetDisableSkillTargeting(bool IsAttackSkill)
{
	if (IsAttackSkill) {
		for (int32 i = 0; i < EnemyCharacters.Num(); i++) {
			//Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetHighLight(true);
			Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetGridOff();
			Cast<AEnemySquadCharacter>(EnemyCharacters[i])->SetGridColor(FColor::Red);
		}
	}
	else {
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetHighLight(false);
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetGridColor(FColor::Green);
	}

}