// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadController.h"
#include "Grid.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "AIController.h"
#include "SquadCharacterSkillComponent.h"
#include "BattleWidget.h"
#include "Engine/Engine.h"
#include "Sound/SoundBase.h"



ASquadController::ASquadController()
{
	bShowMouseCursor = true;

	Curve1 = nullptr;
	LerpTimelineLength = 3.0f;
	YOffset = 800.f;

	testbool = false;

	static ConstructorHelpers::FObjectFinder<USoundBase> SelectedSound(L"SoundWave'/Game/AUDIO/Sound/SFX/button.button'");
	if (SelectedSound.Succeeded())
	{
		Selected_Sound = SelectedSound.Object;
	}
	
}



void ASquadController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundHighlight;
	TArray<AActor*> FoundBattleSystem;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleController::StaticClass(), FoundBattleSystem);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACursorHighlight::StaticClass(), FoundHighlight);

	//Highlight = Cast<ACursorHighlight>(FoundHighlight[0]);
	if(FoundBattleSystem.Num() > 0)
	BattleController = Cast<ABattleController>(FoundBattleSystem[0]);

	SetInputMode(FInputModeGameAndUI());

	APawn* const MyPawn = GetPawn();
			
}

// 예전 버전 시즌2 삭제 예정

void ASquadController::TimelineCallbackTest(float value)
{

	
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);
	auto pSelectedCharController = pSelectedChar->GetController();

	Cast<AAIController>(pSelectedCharController)->MoveToLocation(PathFinder());
}

void ASquadController::TimelineFinishCallbackTest()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum = EStateEnum::SE_End;

	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->EndTurnSystem();
}

FVector ASquadController::PathFinder()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);

	StartLocation = pSelectedChar->GetActorLocation();
	// EndLocation = ;
	FVector MiddleLocation(StartLocation.X, EndLocation.Y, StartLocation.Z);

	if (StartLoc.Y > MiddleLocation.Y)
	{
		if((pSelectedChar->GetActorLocation()).Y  < MiddleLocation.Y + 50.f)
		{
			
			return EndLocation;
		}
	
	}
	else if (StartLoc.Y < MiddleLocation.Y)
	{
		if ((pSelectedChar->GetActorLocation()).Y  > MiddleLocation.Y - 50.f)
		{
			return EndLocation;
		}
	}
	
	return MiddleLocation;
}

void ASquadController::SetHighLight(ACursorHighlight* Cursor)
{
	Highlight = Cursor;
}

// 
void ASquadController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart == true)
		RayHit();
}

void ASquadController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction(TEXT("Move"), EInputEvent::IE_Pressed, this, &ASquadController::MoveCharacter);
	testAB = InputComponent->BindAction(TEXT("SkillButton1"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton1);
	InputComponent->BindAction(TEXT("SkillButton2"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton2);
	InputComponent->BindAction(TEXT("SkillButton3"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton3);
	InputComponent->BindAction(TEXT("SkillButton4"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton4);
	InputComponent->BindAction(TEXT("SkillButton5"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton5);

	InputComponent->BindAction(TEXT("MouseWheel"), EInputEvent::IE_Pressed, this, &ASquadController::SetkeyBindMousewheel);
	InputComponent->BindAction(TEXT("MouseWheelReverse"), EInputEvent::IE_Pressed, this, &ASquadController::SetkeyBindMousewheelreverse);
	InputComponent->BindAction(TEXT("LTab"), EInputEvent::IE_Pressed, this, &ASquadController::SetkeyBindMousewheel);
	InputComponent->BindAction(TEXT("LShift"), EInputEvent::IE_Pressed, this, &ASquadController::SetkeyBindMousewheelreverse);


	
	InputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Pressed, this, &ASquadController::SetTargetCharacter);
	InputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Pressed, this, &ASquadController::SetTargetCharacter_Explorer);
	
	InputComponent->BindAction(TEXT("MoveCharacter"), EInputEvent::IE_Pressed, this, &ASquadController::MoveCharacter);
}

void ASquadController::RayHit() // 타일 변경 시스템
{
	FHitResult RayHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, RayHit);
	RayHit.ImpactPoint;

	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart == true && Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->WhosTurn != true) {
		if (RayHit.bBlockingHit)
		{		
			auto RayHitCharacter = Cast<APlayerSquadCharacter>(RayHit.Actor);

			if (RayHit.Actor == Cast<APlayerSquadCharacter>(RayHit.Actor))  // 플레이어 캐릭터를 선택했을 시
			{						
				//pRayHitCharacter = RayHitCharacter;	

				if(IsGridSelected == true) {	// 캐릭터 선택이 된 상황, 선택되어있는 캐릭터의 타일은 비활성화가 되면 안된다.
					
					APlayerSquadCharacter* SelectedChar = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->GetSelectedCharacter());				

					if (pRayHitCharacter != nullptr && pRayHitCharacter != RayHitCharacter) // 이전 히트 캐릭터와 현재 히트 캐릭터가 다를 경우
					{
						if (SelectedChar->StateEnum == EStateEnum::SE_Stay) {					

							if (pRayHitCharacter != pRayHitSelectedCharacter) {		    // 레이 캐스트 대상 캐릭터가 아닌 다른 오브젝트 일 때
									// 타일을 비활성화
								
							}
							//Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
							//pRayHitCharacter->SetGridOff();
						}					
					}
					else // 이전 히트 캐릭터와 현재 히트 캐릭터가 같을 경우
					{
						if(SelectedChar != nullptr) {
							if ((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
						
								if(Cast<APlayerSquadCharacter>(RayHitCharacter) == SelectedChar) {
									Cast<APlayerSquadCharacter>(RayHitCharacter)->SetHighLight_SelfSkill(true);
								}
								else {
									Cast<APlayerSquadCharacter>(RayHitCharacter)->SetHighLight(true);
								}
							}
							else if (SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1
								|| SelectedChar->StateEnum == EStateEnum::SE_Skill2 || SelectedChar->StateEnum == EStateEnum::SE_Stay) {
								if(pRayHitCharacter != nullptr && Cast<APlayerSquadCharacter>(RayHitCharacter) != SelectedChar)
									Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(true);
							}
						}
						else
						{

						}
						
						//if(false)//pRayHitCharacter->IsGridOn != true)
						// RayHitCharacter->SetGridOn();
						// RayHitCharacter->SetHighLight(true);
						
					}

					pRayHitCharacter = RayHitCharacter; // 대상을 바꾼다
				}
				else 
				{
					/*
					RayHitCharacter->SetGridOn();

					if (RayHitCharacter != nullptr && RayHitCharacter != RayHitCharacter) // 이전 히트 캐릭터와 현재 히트 캐릭터가 다를 경우
					{
						// 레이 캐스트 대상 캐릭터가 아닌 다른 오브젝트 일 때
						RayHitCharacter->SetGridOff();			// 타일을 비활성화
						RayHitCharacter->SetHighLight(false);
					}
					pRayHitCharacter = RayHitCharacter;	
					*/
				}			
			}
			else if (RayHit.Actor == Cast<AEnemySquadCharacter>(RayHit.Actor)) // 적 캐릭터를 선택했을 시
			{
				auto RayHitEnemyCharacter = Cast<AEnemySquadCharacter>(RayHit.Actor);

				pRayHitEnemyCharacter = RayHitEnemyCharacter;
				auto SelectedChar = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->GetSelectedCharacter());
				if (IsGridSelected == true && (SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1 || SelectedChar->StateEnum == EStateEnum::SE_Skill2)) {
					//pRayHitEnemyCharacter->SetGridOn();
					//pRayHitEnemyCharacter->SetGridColor(FColor::Red);
					//RayHitCharacter->SetHighLight(true);
					RayHitEnemyCharacter->SetHighLight(true);
				}
				if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter != nullptr)
				{
					
					Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpdataWidget_TargetCharacterPanel(Cast<AEnemySquadCharacter>(RayHit.Actor));
					auto SelectedChar = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->GetSelectedCharacter());
					if(Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->StateEnum == EStateEnum::SE_Shot) {
						Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->Calc_Damage_distribution(Cast<AEnemySquadCharacter>(RayHit.Actor));
						
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetDamageGraph(SelectedChar);
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHit.Actor), SelectedChar);

			
					}
					else if (Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill1) {
						
						Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->CharacterSkillComp->Calc_SkillData(Cast<AEnemySquadCharacter>(RayHit.Actor), 1);
						
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetDamageGraph(SelectedChar);
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHit.Actor), SelectedChar);

	
						
					}
					else if (Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill2) {

						Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->CharacterSkillComp->Calc_SkillData(Cast<AEnemySquadCharacter>(RayHit.Actor), 2);

						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetDamageGraph(SelectedChar);	
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHit.Actor), SelectedChar);

				
					}
					else if (Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->StateEnum == EStateEnum::SE_Stay || Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter)->StateEnum == EStateEnum::SE_End) {
					
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetDamageGraph_Blank();
						Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHit.Actor), SelectedChar);
					}
				}

			}
		}
		else // 캐릭터에서 마우스가 벗어났을 시 ( 다른 오브젝트에 레이캐스트가 적중했을 시 )
		{
			if(pRayHitCharacter != nullptr && IsGridSelected == false) {		
				//pRayHitCharacter->SetGridOff();

				if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
				{
					//Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
				}
				pRayHitCharacter = nullptr;
			} // 삭제 예정

			if (pRayHitCharacter != pRayHitSelectedCharacter && IsGridSelected == true) {
				if(pRayHitCharacter != nullptr)
				{
					//pRayHitCharacter->SetGridOff();		
					if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
					{
						auto SelectedChar = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->GetSelectedCharacter());
						if ((SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1 || SelectedChar->StateEnum == EStateEnum::SE_Skill2)) {
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
							pRayHitCharacter = nullptr;
						}	
						else if((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight_SelfSkill(false);
						}
						else if ((SelectedChar->StateEnum == EStateEnum::SE_Stay)) {
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
							pRayHitCharacter = nullptr;
						}

					}
					//pRayHitCharacter = nullptr;
				}
			}
			if (pRayHitCharacter == pRayHitSelectedCharacter && IsGridSelected == true) {
				if (pRayHitCharacter != nullptr)
				{
					//pRayHitCharacter->SetGridOff();		
					if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
					{
						auto SelectedChar = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->GetSelectedCharacter());
					
						if ((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(true);
						}

					}
					pRayHitCharacter = nullptr;
				}
			}
			if (pRayHitEnemyCharacter != nullptr && IsGridSelected == true) {
				//pRayHitEnemyCharacter->SetGridOff();
				Cast<AEnemySquadCharacter>(pRayHitEnemyCharacter)->SetHighLight(false);
				if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
				{
					//Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
				}
				pRayHitEnemyCharacter = nullptr;
			}
		}
	}
}

void ASquadController::SetTargetCharacter()
{
	if(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart == true) {
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitTarget);
		if (HitTarget.bBlockingHit == true && Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->WhosTurn == false)
		{
			if (HitTarget.Actor == Cast<ASquadCharacter>(HitTarget.Actor))
			{
				auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

				if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns != nullptr && Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true) // 전투가 시작했을시
				{
					if (HitTarget.Actor == Cast<AEnemySquadCharacter>(HitTarget.Actor) && Cast<AEnemySquadCharacter>(HitTarget.Actor)->StateEnum != EStateEnum::SE_Death) // 적 캐릭터를 선택시
					{ 
						gameIns->TargetCharacter = nullptr;
						gameIns->TargetCharacter = HitTarget.GetActor();


						if (gameIns->SelectedCharacter != nullptr && Cast<ASquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Shot) // 선택된 캐릭터가 Shot 상태 일때
						{
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->Debug_Shot(Cast<ASquadCharacter>(HitTarget.Actor));				
							auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
							SplayerController->SetSquadControllerInput(false);
							DisableInput(SplayerController);
						}
						else if (gameIns->SelectedCharacter != nullptr && Cast<ASquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill1) // 선택된 캐릭터가 Shot 상태 일때
						{
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetSkillNumAndTarget(1, Cast<ASquadCharacter>(HitTarget.Actor));
							auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
							SplayerController->SetSquadControllerInput(false);
							DisableInput(SplayerController);
						}
						else if (gameIns->SelectedCharacter != nullptr && Cast<ASquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill2) // 선택된 캐릭터가 Shot 상태 일때
						{
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetSkillNumAndTarget(2, Cast<ASquadCharacter>(HitTarget.Actor));
							auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
							SplayerController->SetSquadControllerInput(false);
							DisableInput(SplayerController);
						}
					}
					if (HitTarget.Actor == Cast<APlayerSquadCharacter>(HitTarget.Actor) && Cast<APlayerSquadCharacter>(HitTarget.Actor)->StateEnum != EStateEnum::SE_Death) // 아군캐릭터 선택시
					{
						if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Reload && HitTarget.Actor == Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)) {
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->BeReload();
							//Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetHighLight(false);
						}
						else if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Cover && HitTarget.Actor == Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)) {
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetCover();
							//Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetHighLight(false);
						}
						else if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Reload && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Cover){
							ASquadGameMode* gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
							gameMode->ClearCharacterInfoWidgetText_Right();

							if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Stay)
							{
								if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
								{
									Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetStay();
								}				
						
							}

							gameIns->BCIns->SetDisableSkillTargeting(true);
							gameIns->BCIns->SetDisableSkillTargeting(false);
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetHighLight(true);
							
							gameIns->BCIns->SetSelectedCharacter(Cast<ASquadCharacter>(HitTarget.Actor)); // 인스턴스의 SelectedCharacter 초기화 함수
							Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();


							if (pRayHitSelectedCharacter != nullptr && pRayHitSelectedCharacter != Cast<ASquadCharacter>(HitTarget.Actor)) { // 레이캐스트 선택된 캐릭터가 존재하며, 타겟캐릭터와 다를시
								//pRayHitSelectedCharacter->SetGridOff();
								//pRayHitSelectedCharacter->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
								if (pRayHitSelectedCharacter == Cast<APlayerSquadCharacter>(pRayHitSelectedCharacter))
								{
									Cast<APlayerSquadCharacter>(pRayHitSelectedCharacter)->SetHighLight(false);
									Cast<APlayerSquadCharacter>(pRayHitSelectedCharacter)->SetGridOff();
								}
								pRayHitSelectedCharacter = nullptr;

								Cast<ASquadCharacter>(HitTarget.Actor)->SetGridOn();
								//Cast<ASquadCharacter>(HitTarget.Actor)->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
								if ((HitTarget.Actor) == Cast<APlayerSquadCharacter>(HitTarget.Actor))
								{
									Cast<APlayerSquadCharacter>(HitTarget.Actor)->SetHighLight(true);
								}
							}

							if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End) {

								Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetText(Cast<ASquadCharacter>(HitTarget.Actor)); // 위젯을  타겟캐릭터 정보로 초기화
								Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter));
								Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
								IsGridSelected = true;
								pRayHitSelectedCharacter = Cast<ASquadCharacter>(HitTarget.Actor); 
								Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
								//Cast<ASquadCharacter>(HitTarget.Actor)->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
						
							}
							else if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_End) {

								Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetText(Cast<ASquadCharacter>(HitTarget.Actor)); // 위젯을  타겟캐릭터 정보로 초기화
								Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter));
								Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);
								IsGridSelected = true;
								pRayHitSelectedCharacter = Cast<ASquadCharacter>(HitTarget.Actor);
								Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
								/*
								Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ClearCharacterInfoWidgetText();
								Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
								*/
							}
						}

					}
				}
			}
			else // 캐릭터가 아닌 다른 오브젝트를 선택했을시
			{
				/* 기획 변경 삭제 예정
				auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

			
				if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter) != nullptr) // 인스턴스에 SelectedCharacter가 없다면
				{
				


				}
				else if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns != nullptr && Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true)
				{
					if (gameIns->SelectedCharacter != nullptr)
					{
						if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
						{
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetStay();
						}
						gameIns->BCIns->ClearSelectedCharacter();					}
				}
				*/
			}
		}
	}
}

void ASquadController::ClearpRayHitCharacterValue()
{
	pRayHitCharacter = nullptr;
}

void ASquadController::SetTargetCharacter_Explorer()
{
	if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->IsBattleStart == false) {
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitTarget);
		if (HitTarget.bBlockingHit == true)
		{
			if (HitTarget.Actor == Cast<APlayerSquadCharacter>(HitTarget.Actor))
			{
				ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
				
				if (Target_Explorer != nullptr) { // 타겟이 있을때
				
					//preTarget_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
					preTarget_Explorer = Target_Explorer;
					preTarget_Explorer->SetHighLight(false);
					
					Target_Explorer = Cast<APlayerSquadCharacter>(HitTarget.Actor);
					//Target_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
					Target_Explorer->SetHighLight(true);
					BC->SetSelectedCharacter(Target_Explorer);

					Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();

					//preTarget_Explorer = Target_Explorer;
				}
				else{ // 타겟이 없을때
					auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
									
					Target_Explorer = Cast<APlayerSquadCharacter>(HitTarget.Actor);
					Target_Explorer->SetHighLight(true);
					BC->SetSelectedCharacter(Target_Explorer);

					//Target_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
					//preTarget_Explorer = Target_Explorer;
					Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();

				}	

				Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
				//Cast<APlayerSquadCharacter>(HitTarget.Actor)->DebugMessage_CharacterState();
			}


		}
		SetSelectedCharacterInfo();
	}
}

void ASquadController::MoveCharacter()
{
	FHitResult HitPos;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitPos);
	HitPos.ImpactPoint;

	
	if (HitPos.bBlockingHit)
	{
		//MoveToMouseCursor(HitPos.ImpactPoint);
	
		
		if (HitPos.Actor == Cast<AGrid>(HitPos.Actor))
		{
			auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

			auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);
			auto pGrid = Cast<AGrid>(HitPos.Actor);

		
			if (pSelectedChar->pGridOnCharacter != nullptr)
			{
				pSelectedChar->pGridOnCharacter->GridInfo.GOTO = EGridOntheObject::Normal;
				pSelectedChar->pGridOnCharacter->SetGridInfo_Material();
			}
			
			FVector SelectedCharacterLoc = pSelectedChar->GetActorLocation();
			FVector pGridLoc = pGrid->GetActorLocation();
			
			EndLocation = pGridLoc;

			StartLoc = pSelectedChar->GetActorLocation();
			
			LerpTimeline.PlayFromStart();

			pGrid->GridInfo.GOTO = EGridOntheObject::Player;
			pGrid->SetGridInfo_Material_Black(); // 색변환 함수 임시
			pSelectedChar->pGridOnCharacter = pGrid;


		}
		else
		{
			




		}
		
	}	


}

void ASquadController::MoveToMouseCursor(const FVector Location)
{
	APawn* const MyPawn = GetPawn();

	FVector PawnLoc = MyPawn->GetActorLocation();
	FVector DesLoc(Location.X, Location.Y, 0.0f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DesLoc);

	
}

int32 ASquadController::SwitchGamePlayValue()
{
	if (IsGamePlay == 0) IsGamePlay = 1;
	else if (IsGamePlay == 1) IsGamePlay = 0;

	return IsGamePlay;
}


/*

void AGamePlayerController::HitYPosistion()
{
	FHitResult HitPos;
	GetHitResultUnderCursor(ECC_Visibility, false, HitPos);
	if (HitPos.bBlockingHit)
	{
		MoveToMouseCursor(HitPos.ImpactPoint);
		UE_LOG(LogClass, Log, TEXT("work "));
	}

	UE_LOG(LogClass, Log, TEXT("work2 "));
}

void AGamePlayerController::MoveToMouseCursor(const FVector Location)
{
	APawn* const MyPawn = GetPawn();

	FVector PawnLoc = MyPawn->GetActorLocation();
	FVector DesLoc(PawnLoc.X, Location.Y, 0.0f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DesLoc);


}

*/

// Key Bind & UnBind

void ASquadController::Debug_TurnSystem1()
{	
	
	for (int i = 0; i < InputComponent->GetNumActionBindings(); i++)
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (CompareInputActionBindings(testAB, Binding))
		{
			InputComponent->RemoveActionBinding(i);
			i--;
			continue;
		}
	}

	testAB = InputComponent->BindAction(TEXT("F12"), EInputEvent::IE_Pressed, this, &ASquadController::Debug_TurnSystem2);
		

}

void ASquadController::Debug_TurnSystem2()
{
	// 하드코딩 스타일의 언바운드 & 바운드
	
	for (int i = 0; i < InputComponent->GetNumActionBindings(); i++)
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (CompareInputActionBindings(testAB, Binding))
		{
			InputComponent->RemoveActionBinding(i);
			i--;
			continue;
		}
	}

	testAB = InputComponent->BindAction(TEXT("F12"), EInputEvent::IE_Pressed, this, &ASquadController::Debug_TurnSystem1);
}


//////////////////

void ASquadController::SetKeyBindSkillButton1() // 사격버튼
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (1)
	{
		
		if (AActor* SC = BC->GetSelectedCharacter())
		{
			
			Cast<APlayerSquadCharacter>(SC)->SetShotReady();
		}
		
			
	}
	else
	{
		
	}
}

void ASquadController::SetKeyBindSkillButton2() // 스킬1버튼
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (1)	{
	
		if (AActor* SC = BC->GetSelectedCharacter())	{
			Cast<APlayerSquadCharacter>(SC)->SetSkill1();
		}

	}

}

void ASquadController::SetKeyBindSkillButton3() // 스킬2버튼
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (1) {
		if (AActor* SC = BC->GetSelectedCharacter()) {
		
			Cast<APlayerSquadCharacter>(SC)->SetSkill2();
		}
	}
}

void ASquadController::SetKeyBindSkillButton4() // 장전버튼
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (1)	{
		
		if (AActor* SC = BC->GetSelectedCharacter()) {
			
			Cast<APlayerSquadCharacter>(SC)->SetReloadReady();
		}	
			
	}

}

void ASquadController::SetKeyBindSkillButton5() // 엄폐버튼
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (1)
	{

		if (AActor* SC = BC->GetSelectedCharacter()) {
			
			Cast<APlayerSquadCharacter>(SC)->SetCoverReady();
		}

	}

}



void ASquadController::SetkeyBindMousewheel()
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (BC->IsBattleStart) {
		if (AActor* SC = BC->GetSelectedCharacter()) {
		
			BC->ReverseControlChangeSelectedCharacter();
			Cast<APlayerSquadCharacter>(BC->GetSelectedCharacter())->PlaySelectedSound();
		}
	}
	else {
		ControlTarget_ExplorerWheel();
	}
	//UGameplayStatics::PlaySound2D(this, Selected_Sound);
}

void ASquadController::SetkeyBindMousewheelreverse()
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (BC->IsBattleStart) {
		if (AActor* SC = BC->GetSelectedCharacter()) {
			
			BC->ControlChangeSelectedCharacter();
			Cast<APlayerSquadCharacter>(BC->GetSelectedCharacter())->PlaySelectedSound();
		}
	}
	else {
		ControlTarget_ExplorerWheelReverse();
	}
	//UGameplayStatics::PlaySound2D(this, Selected_Sound);
}

void ASquadController::ControlTarget_ExplorerWheel()
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if(Target_Explorer) {
		auto list = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->FriendlyCharList;
		int32 tempNum;
		for (int i = 0; i < list.Num(); i++) {
			if (Cast<APlayerSquadCharacter>(list[i]) == Target_Explorer) { tempNum = i + 1; }
			if (tempNum == list.Num()) { tempNum = 0; }
		}

		Cast<APlayerSquadCharacter>(list[tempNum])->SetHighLight(true);
		preTarget_Explorer = Target_Explorer;
		preTarget_Explorer->SetHighLight(false);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[tempNum]);
		BC->SetSelectedCharacter(Target_Explorer);
	}
	else {
		auto list = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->FriendlyCharList;
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[0]);		
		BC->SetSelectedCharacter(Target_Explorer);
	}

	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	Cast<APlayerSquadCharacter>(BC->GetSelectedCharacter())->PlaySelectedSound();
	SetSelectedCharacterInfo();
}

void ASquadController::ControlTarget_ExplorerWheelReverse()
{
	ABattleController* BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	if (Target_Explorer) {
		auto list = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->FriendlyCharList;
		int32 tempNum;
		for (int i = 0; i < list.Num(); i++) {
			if (Cast<APlayerSquadCharacter>(list[i]) == Target_Explorer) { tempNum = i - 1; }
			if (tempNum == -1 ) { tempNum = list.Num() - 1; }
		}

		Cast<APlayerSquadCharacter>(list[tempNum])->SetHighLight(true);
		preTarget_Explorer = Target_Explorer;
		preTarget_Explorer->SetHighLight(false);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[tempNum]);
		BC->SetSelectedCharacter(Target_Explorer);
	}
	else {
		auto list = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->FriendlyCharList;
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[0]);
		BC->SetSelectedCharacter(Target_Explorer);
	}

	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->Init_SkillButtonColor();
	Cast<APlayerSquadCharacter>(BC->GetSelectedCharacter())->PlaySelectedSound();
	SetSelectedCharacterInfo();
}

// 

void ASquadController::CharacterMove()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());


}

void ASquadController::EmptypRayHitCharacter(ASquadCharacter* CurrentChar)
{
	pRayHitCharacter = CurrentChar;
	pRayHitSelectedCharacter = CurrentChar;
}

bool ASquadController::CompareInputActionBindings(FInputActionBinding lhs, FInputActionBinding rhs) // lhs와 rhs가 같은 액션바인딩인지 비교
{
	return lhs.ActionDelegate.GetDelegateWithKeyForManualSet().GetHandle() ==
		rhs.ActionDelegate.GetDelegateWithKeyForManualSet().GetHandle() &&
		lhs.GetActionName() == rhs.GetActionName();
}

void ASquadController::ClearSelectedCharacter_Explorer()
{
	if(Target_Explorer != nullptr) {
		
		//Target_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
		Target_Explorer->SetHighLight(false);

		Target_Explorer = nullptr;
		preTarget_Explorer = nullptr;
	}

}

void ASquadController::SetSquadControllerInput(bool bIsStop)
{
	if (bIsStop == false) {
		DisableInput(this);
	}
	else {
		EnableInput(this);
	}
}

void ASquadController::SetSelectedCharacterInfo()
{
	auto list = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->FriendlyCharList;
	auto bc = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	auto bm = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	if (bm->GetCurrentWidget() == Cast<UBattleWidget>(bm->GetCurrentWidget()))
		Cast<UBattleWidget>(bm->GetCurrentWidget())->ClearWidget_SkillPart();

	if(bc->GetSelectedCharacter() == nullptr) {
		bc->SetSelectedCharacter(Cast<APlayerSquadCharacter>(list[0]));
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		bm->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(list[0]));
		bm->UpDateWidgetText(Cast<APlayerSquadCharacter>(list[0]));
	}
	else {
		APlayerSquadCharacter* PC = Cast<APlayerSquadCharacter>(bc->GetSelectedCharacter());
		bm->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(PC));
		bm->UpDateWidgetText(Cast<APlayerSquadCharacter>(PC));
	
	}

	bm->SetBattleWidgetSkillButtonOpacity_transparent();

}

void ASquadController::SetMenuInput(bool bIsStop)
{
	auto SCM = Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns);
	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());

	if(bIsStop == true) {
		SplayerController->SetSquadControllerInput(false);
		DisableInput(SplayerController);
		SCM->ControlValue_PlayerCharacterMovement = false;
	
	}
	else {
		SplayerController->SetSquadControllerInput(true);
		EnableInput(SplayerController);
		SCM->ControlValue_PlayerCharacterMovement = true;
	}
}