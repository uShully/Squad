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

void ABattleController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

// Called when the game starts or when spawned
void ABattleController::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 인스턴스 초기화
	gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameIns->BCIns = this;
	// 게임모드 초기화
	gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	// 플레이어 컨트롤러 초기화
	SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());

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
	// triger box 초기화
	if (pTriggerBox == nullptr)  pTriggerBox = Cast<ABattleTrigger>(triggerBox);

	ClearArray();

	// 플레이어, 적 캐릭터 탐색
	GetFriendlyCharacters(); 
	GetEnemyCharacters(EnemyList); 

	// 모든 캐릭터 정렬
	SortCharacters();

	SystemState.init();

	PlayerDeathCount = 0;
	EnemyDeathCount = 0;
	
	if (AllCharacters.Num() > 0)
	{
		SystemState.CharNumber = AllCharacters.Num();
	}

	StartTurnSystem_init();
}

void ABattleController::GetFriendlyCharacters()
{
	FriendlyCharacters.Append(gameIns->SCMIns->FriendlyCharList);
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
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->ArrayNumbering = i;
		Cast<AEnemySquadCharacter>(EnemyCharacters[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
	}
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
////////////////////////////////////////////                ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ABattleController::StartTurnSystem_init() 
{
	SetIsBattleStart(true);

	// UI	
	gameMode->ViewBattleWidget();	

	if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

	Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
	Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
		
	// 전투 초기화 - 게임 인스턴스 변수 제어
	gameIns->SelectedCharacter = nullptr;
	gameIns->IsBattleStart = true;
	
	// 전투 초기화 - 탐색시 선택된 캐릭터 초기화
	SplayerController->ClearSelectedCharacter_Explorer();
	// 전투 초기화 - 선택된 캐릭터 초기화
	gameIns->SelectedCharacter = SystemState.SelectedCharacter;
	// 전투 초기화 - 카메라 제어
	gameIns->SCMIns->zoomswitch();
	gameIns->SCMIns->IsExploreToBattle = true;
	gameIns->SCMIns->Control_SetExploreInit(gameIns->SCMIns->GetActorLocation());
	gameIns->SCMIns->Control_SetBattleInit(gameIns->BTIns->GetNeturalAreaLocation());
	gameIns->SCMIns->MoveSwitch = true;

	// 키 입력 정지
	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);

	// 캐릭터 산개
	gameIns->BTIns->BattleTrigger_PlayerSpreadOut();
	
	// 람다식 - 1.5초 후 턴 시작 시스템 호출
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			StartTurnSystem();

		}), 1.5f, false);

	
	// 적군 전투 배열 초기화
	EnemyStartBattleArray = (EnemyCharacters);
	
	// BGM 재생
	BGMComp->SetSound(BattleSound);
	BGMComp->FadeIn(3.f);	
}

void ABattleController::ControlCharacterCameraMovement(bool PlayerMovementSwitch)
{	// true 면 움직임 false면 안움직임 
	// 카메라 변수는 false , 캐릭터 변수는 true일때 움직임		
	gameIns->SCMIns->Control_PlayerCharacterMovement(PlayerMovementSwitch);
	gameIns->SCMIns->Control_CameraMovement(!PlayerMovementSwitch);
}

bool ABattleController::SwitchCharacterMovement(bool PlayerMovementSwitch)
{
	gameIns->SCMIns->Control_PlayerCharacterMovement(PlayerMovementSwitch);
	
	return gameIns->SCMIns->ControlValue_CameraMovement;
}

void ABattleController::StartTurnSystem()
{	
	SplayerController->SetSquadControllerInput(true);
	EnableInput(SplayerController);

	// 모든 캐릭터 상태를 턴 종료 상태로 변경
	for (int32 i = 0; i < AllCharacters.Num(); i++)
	{
		Cast<ASquadCharacter>(AllCharacters[i])->StateEnum = EStateEnum::SE_End;
	}	
	
	// 플레이어가 선공이면 아군 캐릭터는 대기모드, 적군 캐릭터는 종료모드
	if (!WhosTurn) 
	{
		gameMode->ChangeWhosTurnName(WhosTurn);
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}
	}
	else // 적 캐릭터 선공시 아군 캐릭터는 종료모드, 적군 캐릭터는 대기모드
	{
		gameMode->ChangeWhosTurnName(WhosTurn);
		for (int32 i = 0; i < EnemyCharacters.Num(); i++)
		{
			Cast<ASquadCharacter>(EnemyCharacters[i])->StateEnum = EStateEnum::SE_Stay;
		}
	}

	// 전투용 애니메이션 호출
	for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
	{
		auto anim = Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->animInstance;
		Cast<UCharacterAnimInstance>(anim)->Set_IsBattle(true);
		Cast<UCharacterAnimInstance>(anim)->Call_GetIsBattle();
	}

	// 전투 위젯 초기화
	Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();
	   
	// 전투 시작시 현재 선택된 캐릭터로 전투 정보 초기화
	APlayerSquadCharacter* SelectedChar = Cast<APlayerSquadCharacter>(GetSelectedCharacter());
	SetSelectedCharacter(Cast<ASquadCharacter>(SelectedChar));

	SelectedChar->SetHighLight(true);
	SelectedChar->SetGridOn();

	SplayerController->SetpRayHitCharacter(SelectedChar);
	SplayerController->SetpRayHitSelectedCharacter(SelectedChar);

	gameMode->UpDateWidgetText_Right2(SelectedChar);
	gameMode->UpDateWidgetText(SelectedChar);

	Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
	Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
	SelectedChar->PlaySelectedSound();
}

void ABattleController::SetSelectedCharacter(ASquadCharacter* SelectedChararcter)
{
	
	if (SelectedChararcter == nullptr) {
		SystemState.SelectedCharacter = nullptr;
		gameIns->SelectedCharacter = SystemState.SelectedCharacter;
	}
	else {
		SystemState.SelectedCharacter = SelectedChararcter;
		gameIns->SelectedCharacter = SystemState.SelectedCharacter;
			
		APlayerSquadCharacter* pSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
		gameMode->ChangeSkillButtonWidgetImage(pSelectedCharacter);
	}	
}

void ABattleController::ClearSelectedCharacter()
{
	SystemState.SelectedCharacter = nullptr;
	gameIns->SelectedCharacter = SystemState.SelectedCharacter;

	FString name = TEXT("nullptr");
	gameMode->ChangeCurrentCharName(name);
}

AActor* ABattleController::GetSelectedCharacter()
{
	if (SystemState.SelectedCharacter != nullptr)
		return SystemState.SelectedCharacter;
	else
		return nullptr;
}

void ABattleController::EndTurnSystem()
{
	// 전투 종료시 상황 검사 함수 호출
	BeCheck();

	if (!WhosTurn && IsBattleStart == true)
	{
		if (Cast<ASquadCharacter>(SystemState.SelectedCharacter)->IsGridOn) {	
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOff();
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(false);
		}

						
		SetDisableSkillTargeting(true);
		SetDisableSkillTargeting(false);

		gameMode->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		SystemChangeSelectedCharacter();
		if(SystemState.SelectedCharacter != nullptr) {
			gameMode->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
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
				gameMode->ChangeWhosTurnName(WhosTurn);
				SplayerController->SetSquadControllerInput(false);
				
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
					Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
					EndTurnSystem_Enemy();						
				}			
			}						
	}
}

void ABattleController::EndTurnSystem_Enemy()
{
	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);	

	for (int32 i = 0; i < EnemyCharacters.Num(); i++)
	{
		Cast<ASquadCharacter>(EnemyCharacters[i])->Control_CCArray();			// 스킬 효과 가감 함수 추가 예정 12/22
	}

	gameMode->Set_BattleWidgetOpacity(0.5f); // 위젯 투명도

	if (WhosTurn) 
	{
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				ActiveEnemyAI();

			}), WaitTime, false);
		

	}
}

void ABattleController::BeCheck()
{
	if (EnemyDeathCount == EnemyCharacters.Num()) // 적의 수와 적의 죽은수가 같으면
	{
		if (gameIns->BTIns->BTState == EBattleTriggerState::Boss) // 보스 스테이지 일때
		{
			BGMComp->Stop();
			gameMode->ViewGameVictoryWidget();
		}
		else { 
			ResultBattle_BattleEnd();

			gameMode->ChangeSubWidget(gameMode->GetVictoryWidgetClass());
			gameMode->ClearCharacterInfoWidgetText_Right();
			Cast<ASquadController>(GetWorld()->GetFirstPlayerController())->Target_Explorer = Cast<APlayerSquadCharacter>(GetSelectedCharacter());			
		}				
	}
	else if(PlayerDeathCount == FriendlyCharacters.Num()) // 플레이어 전멸
	{
		// UI
		ResultBattle_BattleEnd();
		BGMComp->Stop();
		gameMode->ChangeMenuWidget(gameMode->GetDefeatWidgetClass());
	}
}

// 전투 리스트에서


void ABattleController::ActiveEnemyAI()
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
	else if(EnemyEndBattleArray.Num() != EnemyCharacters.Num() - (EnemyDeathCount)) // 그렇지 않다면 전투리스트 인덱스 0번째의 적 캐릭터를 팝한다.
	{
		AEnemySquadCharacter* Enemy = Cast<AEnemySquadCharacter>(EnemyStartBattleArray[0]);
		EnemyStartBattleArray.RemoveSingle(Enemy);
		ASquadAIController* AIcontroller = Cast<ASquadAIController>(Enemy->GetController());		
		
	
		if(PlayerStartBattleArray.Num() == 0)
			AIcontroller->EnemyChararacter_SetFrindlyCharacterList(PlayerEndBattleArray);
		else
			AIcontroller->EnemyChararacter_SetFrindlyCharacterList(PlayerStartBattleArray);

		EnemyEndBattleArray.Push(Enemy);

		AIcontroller->EnemyCharacter_ActiveAI();
	

		if(Enemy->GetUnderGrid() != nullptr)
		Enemy->GetUnderGrid()->SetGridInfo_Material_Black(); 
		

		// 전투를 실행시킨다
		// 전투가 끝나면 전투종료리스트에 푸쉬한다.

	}
	else if(EnemyEndBattleArray.Num() == EnemyCharacters.Num() - (EnemyDeathCount))
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

		SplayerController->SetSquadControllerInput(true);
		EnableInput(SplayerController);

		WhosTurn = false;
		gameMode->ChangeWhosTurnName(WhosTurn);
		gameMode->Set_BattleWidgetOpacity(1.f);
		SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));

		
		gameMode->UpDateWidgetText(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
		gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			if(Cast<ASquadCharacter>(FriendlyCharacters[i])->StateEnum != EStateEnum::SE_Death)
			Cast<ASquadCharacter>(FriendlyCharacters[i])->Control_CCArray();		// 스킬 효과 가감 함수 추가 예정 12/22
		}
		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter) {
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();

			SplayerController->SetpRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			SplayerController->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		}
		Cast<APlayerSquadCharacter>(PlayerStartBattleArray[0])->PlaySelectedSound();

		if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();

		SplayerController->SetSquadControllerInput(true);
	}
}

// Enemy Start & End Battle Array

void ABattleController::RemoveFromEnemyEndBattleArray(AActor* RemoveUnit)
{
	if(EnemyEndBattleArray.Num() > 0)
	{ 
		EnemyEndBattleArray.RemoveSingle(RemoveUnit);
		for(int32 i = 0 ; i < EnemyEndBattleArray.Num() ; i++)
		Cast<AEnemySquadCharacter>(EnemyEndBattleArray[i])->ArrayNumbering = i;
	}
	else if (EnemyStartBattleArray.Num() > 0) // 첫턴에 원턴킬 날때 생기는 오류 보정 - 만약 적이 그럴 수 있는 상황이 생기면 아군 배열에도 추가해야함
	{
		EnemyStartBattleArray.RemoveSingle(RemoveUnit);
		for (int32 i = 0; i < EnemyStartBattleArray.Num(); i++)
			Cast<AEnemySquadCharacter>(EnemyStartBattleArray[i])->ArrayNumbering = i;
	}
}

void ABattleController::RemoveFromPlayerEndBattleArray(AActor* RemoveUnit)
{
	PlayerEndBattleArray.RemoveSingle(RemoveUnit);
	for (int32 i = 0; i < PlayerEndBattleArray.Num(); i++)
		Cast<APlayerSquadCharacter>(PlayerEndBattleArray[i])->ArrayNumbering = i;

	gameIns->SCMIns->FriendlyCharList.RemoveSingle(RemoveUnit);
	for (int32 i = 0; i < gameIns->SCMIns->FriendlyCharList.Num(); i++)
		Cast<APlayerSquadCharacter>(gameIns->SCMIns->FriendlyCharList[i])->numbering = i;
	PlayerDeathCount++;
}

void ABattleController::AddPlayerEndBattleArray(AActor* Actor)
{

	auto tempPlayerChar = Cast<APlayerSquadCharacter>(Actor);

	PlayerStartBattleArray.RemoveAt(tempPlayerChar->ArrayNumbering);
	for (int32 i = 0; i < PlayerStartBattleArray.Num(); i++)
		Cast<APlayerSquadCharacter>(PlayerStartBattleArray[i])->ArrayNumbering = i;

	PlayerEndBattleArray.Add(tempPlayerChar);
	tempPlayerChar->ArrayNumbering = PlayerEndBattleArray.Num() - 1;

}

void ABattleController::ResetPlayerEndBattleArray()
{
	if (PlayerEndBattleArray.Num() == FriendlyCharacters.Num() - PlayerDeathCount)
	{
		PlayerStartBattleArray.Append(PlayerEndBattleArray); // 행동 끝낸 아군 캐릭터들을 다시 행동력을 채우고
		PlayerEndBattleArray.Empty();              // EndArray를 비운다

	}
}

void ABattleController::AddEnemyDeathCount()
{
	EnemyDeathCount++;
}

void ABattleController::ResultBattle_PostInit()
{
	// 전투 종료시 카메라 위치 제어
	gameIns->SCMIns->zoomswitch();
	gameIns->SCMIns->IsBattleToExplore = true;
	gameIns->SCMIns->MoveSwitch = true;


	// Sound 제어
	BGMComp->FadeOut(3.f , 0.f);
	BGMComp->SetSound(ExplorerSound);
	BGMComp->FadeIn(2.f);
}

void ABattleController::ResultBattle_BattleEnd()
{
	if(IsBattleStart == true) { 

		// 전투 종료(플레이어 승리)시 탐색 상태로 변경

		// 전투위젯 스킬 아이콘 비활성화
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);

		// 아군 캐릭터 탐색 상태로 변경
		for (int32 i = 0; i < FriendlyCharacters.Num(); i++)
		{
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->BeReload_BattleOver();
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->Clear_CCArray();

			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetCharacterMovement()->MaxWalkSpeed = 200.f;
			Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->GetCharacterMovement()->MaxAcceleration = 150.f;

			auto anim = Cast<APlayerSquadCharacter>(FriendlyCharacters[i])->animInstance;
			Cast<UCharacterAnimInstance>(anim)->Set_IsBattle(false);
			Cast<UCharacterAnimInstance>(anim)->Call_GetIsBattle();
		}
		// 캐릭터 탐색 위치 초기화
		gameIns->SCMIns->SetUnitPos_Last_Location(gameIns->SCMIns->FriendlyCharList.Num());
		
		ClearArray();

		SetIsBattleStart(false);
		gameIns->IsBattleStart = GetIsBattleStart();

		int32 LastUnit_Loc = gameIns->SCMIns->FriendlyCharList.Num();

		gameIns->BTIns = nullptr;

		pTriggerBox->DeleteBattleTrigger();
		pTriggerBox = nullptr;

		EnemyDeathCount = 0;
		PlayerDeathCount = 0;
	}
}

void ABattleController::ChangeSelectedCharacter()
{		
	if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->StateEnum == EStateEnum::SE_End) {
		auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
		preSelectedCharacter->SetHighLight(false);
		
		if(Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering == FriendlyCharacters.Num() - 1)
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[0]));
		else
			SetSelectedCharacter(Cast<APlayerSquadCharacter>(FriendlyCharacters[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->numbering + 1]));
				
		SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
		SplayerController->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
		SplayerController->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));

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
			
				SplayerController->SetpRayHitCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
				SplayerController->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
				Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
				SplayerController->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			
				AddPlayerEndBattleArray(preSelectedCharacter);			
			}
			else {
				auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
				preSelectedCharacter->SetHighLight(false);			
			
				SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
				SplayerController->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
				SplayerController->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
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

	if (PlayerStartBattleArray.Num() > 1) {
		if (PlayerStartBattleArray.Num() - 1 == Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering) {
			auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
			preSelectedCharacter->SetHighLight(false);

			SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			SplayerController->SetpRayHitCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			SplayerController->SetpRayHitSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[0]));
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();
			SplayerController->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
			AddPlayerEndBattleArray(preSelectedCharacter);
		}
		else {
			auto preSelectedCharacter = Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter);
			preSelectedCharacter->SetHighLight(false);
			
			SetSelectedCharacter(Cast<ASquadCharacter>(PlayerStartBattleArray[Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->ArrayNumbering + 1]));
			SplayerController->SetpRayHitCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
			SplayerController->SetpRayHitSelectedCharacter(Cast<APlayerSquadCharacter>(GetSelectedCharacter()));
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->SetGridOn();

			AddPlayerEndBattleArray(preSelectedCharacter);
		}

			Cast<APlayerSquadCharacter>(GetSelectedCharacter())->PlaySelectedSound();
			Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
			if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
				Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);

			if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
				Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
	}
	else {
		Cast<APlayerSquadCharacter>(GetSelectedCharacter())->SetHighLight(false);
		AddPlayerEndBattleArray(SystemState.SelectedCharacter);
		SetSelectedCharacter(nullptr);
	}
}


void ABattleController::ReverseControlChangeSelectedCharacter()
{
	auto preSelectedCharacter = SystemState.SelectedCharacter;
	int32 tempValue = 0;

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
		gameMode->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		gameMode->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);


		if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
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
		SplayerController->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		gameMode->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시
		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);
	
		if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
	}
}

void ABattleController::ControlChangeSelectedCharacter()
{
	int32 tempValue = 0;

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
		gameMode->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		gameMode->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);


		if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();

		
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();

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
		SplayerController->EmptypRayHitCharacter(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->ClearCharacterInfoWidgetText(); // 아군 턴종료후 UI 초기화 함수
		gameMode->UpDateWidgetText(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		gameMode->UpDateWidgetText_Right2(Cast<ASquadCharacter>(SystemState.SelectedCharacter));
		Cast<ASquadCharacter>(SystemState.SelectedCharacter)->GetStatustBarWidget()->SetBarRenderOpacity(1.f); // 임시

		if (Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter) == SystemState.SelectedCharacter)
			Cast<APlayerSquadCharacter>(SystemState.SelectedCharacter)->SetHighLight(true);

		if (gameMode->GetCurrentWidget() == Cast<UBattleWidget>(gameMode->GetCurrentWidget()))
			Cast<UBattleWidget>(gameMode->GetCurrentWidget())->ClearWidget_SkillPart();
				
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
		Cast<UBattleWidget>(gameMode->GetCurrentWidget())->Init_SkillButtonColor();
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