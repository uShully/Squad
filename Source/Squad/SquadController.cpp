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

	YOffset = 800.f;


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


	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleController::StaticClass(), FoundBattleSystem);
	
	//if (FoundBattleSystem.Num() > 0)
	gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	BattleController = gameIns->BCIns;//Cast<ABattleController>(FoundBattleSystem[0]);
	SquadGameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	if (BattleController != nullptr) {
		SGLOG_S(Warning);
	}

	SetInputMode(FInputModeGameAndUI());

	APawn* const MyPawn = GetPawn();
			
}

void ASquadController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(gameIns->IsBattleStart == true)
		RayHit();
}

void ASquadController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction(TEXT("Move"), EInputEvent::IE_Pressed, this, &ASquadController::MoveCharacter);
	InputComponent->BindAction(TEXT("SkillButton1"), EInputEvent::IE_Pressed, this, &ASquadController::SetKeyBindSkillButton1);
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

}

void ASquadController::RayHit()
{
	RayHitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, RayHitResult);

	if (BattleController->WhosTurn != true) {
		if (RayHitResult.bBlockingHit)
		{		
			RayHitCharacter = Cast<APlayerSquadCharacter>(RayHitResult.Actor);

			if (RayHitResult.Actor == Cast<APlayerSquadCharacter>(RayHitResult.Actor))  // 플레이어 캐릭터를 선택했을 시
			{						
				// 캐릭터 선택이 된 상황, 선택되어있는 캐릭터의 타일은 비활성화가 되면 안된다.
					
				APlayerSquadCharacter* SelectedChar = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());

				if (pRayHitCharacter == nullptr && pRayHitCharacter == RayHitCharacter) // 	// 이전 히트 캐릭터와 현재 히트 캐릭터가 같을 경우			
				{
					if(SelectedChar != nullptr) {
						if ((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
						
							if(Cast<APlayerSquadCharacter>(RayHitCharacter) == SelectedChar) {
								if (RayHitCharacter->GetbIsSelfHighLight() != true)
								RayHitCharacter->SetHighLight_SelfSkill(true);
							}
							else {
								if(RayHitCharacter->GetbIsHighLight() != true)
									RayHitCharacter->SetHighLight(true);
							}
						}
						else if (SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1
							|| SelectedChar->StateEnum == EStateEnum::SE_Skill2 || SelectedChar->StateEnum == EStateEnum::SE_Stay) {
							if(pRayHitCharacter != nullptr && Cast<APlayerSquadCharacter>(RayHitCharacter) != SelectedChar)
								if (Cast<APlayerSquadCharacter>(pRayHitCharacter)->GetbIsHighLight() != true)
								Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(true);
						}
					}								
					pRayHitCharacter = RayHitCharacter; // 대상을 바꾼다
				}
		
			}
			else if (RayHitResult.Actor == Cast<AEnemySquadCharacter>(RayHitResult.Actor)) // 적 캐릭터를 선택했을 시
			{
				pRayHitEnemyCharacter = Cast<AEnemySquadCharacter>(RayHitResult.Actor);
				auto SelectedChar = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());
				if ((SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1 || SelectedChar->StateEnum == EStateEnum::SE_Skill2)) {
					if (pRayHitEnemyCharacter->GetbIsHighLight() != true)
					pRayHitEnemyCharacter->SetHighLight(true);
				}
				if (gameIns->SelectedCharacter != nullptr)	{
					
					SquadGameMode->UpdataWidget_TargetCharacterPanel(Cast<AEnemySquadCharacter>(RayHitResult.Actor));
					auto SelectedChar = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());
					if(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Shot) {
						Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->Calc_Damage_distribution(Cast<AEnemySquadCharacter>(RayHitResult.Actor));
						
						SquadGameMode->UpDateWidgetDamageGraph(SelectedChar);
						SquadGameMode->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHitResult.Actor), SelectedChar);

			
					}
					else if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill1) {
						
						Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->CharacterSkillComp->Calc_SkillData(Cast<AEnemySquadCharacter>(RayHitResult.Actor), 1);
						
						SquadGameMode->UpDateWidgetDamageGraph(SelectedChar);
						SquadGameMode->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHitResult.Actor), SelectedChar);

	
						
					}
					else if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Skill2) {

						Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->CharacterSkillComp->Calc_SkillData(Cast<AEnemySquadCharacter>(RayHitResult.Actor), 2);

						SquadGameMode->UpDateWidgetDamageGraph(SelectedChar);	
						SquadGameMode->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHitResult.Actor), SelectedChar);

				
					}
					else if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Stay || Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_End) {
					
						SquadGameMode->UpDateWidgetDamageGraph_Blank();
						SquadGameMode->UpdateWidget_TargetCharacterStatPanel(Cast<AEnemySquadCharacter>(RayHitResult.Actor), SelectedChar);
					}
				}

			}
		}
		else // 캐릭터에서 마우스가 벗어났을 시 ( 다른 오브젝트에 레이캐스트가 적중했을 시 )
		{
			if (pRayHitCharacter != pRayHitSelectedCharacter) {
				if(pRayHitCharacter != nullptr)
				{
					//pRayHitCharacter->SetGridOff();		
					if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
					{
						auto SelectedChar = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());
						if ((SelectedChar->StateEnum == EStateEnum::SE_Shot || SelectedChar->StateEnum == EStateEnum::SE_Skill1 || SelectedChar->StateEnum == EStateEnum::SE_Skill2)) {
							if (Cast<APlayerSquadCharacter>(pRayHitCharacter)->GetbIsHighLight() != false)
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
							pRayHitCharacter = nullptr;
						}	
						else if((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
							if (Cast<APlayerSquadCharacter>(pRayHitCharacter)->GetbIsSelfHighLight() != false)
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight_SelfSkill(false);
						}
						else if ((SelectedChar->StateEnum == EStateEnum::SE_Stay)) {
							if (Cast<APlayerSquadCharacter>(pRayHitCharacter)->GetbIsHighLight() != false)
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(false);
							pRayHitCharacter = nullptr;
						}

					}
					//pRayHitCharacter = nullptr;
				}
			}
			if (pRayHitCharacter == pRayHitSelectedCharacter) {
				if (pRayHitCharacter != nullptr)
				{
					//pRayHitCharacter->SetGridOff();		
					if (pRayHitCharacter == Cast<APlayerSquadCharacter>(pRayHitCharacter))
					{
						auto SelectedChar = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());
					
						if ((SelectedChar->StateEnum == EStateEnum::SE_Cover || SelectedChar->StateEnum == EStateEnum::SE_Reload)) {
							if (Cast<APlayerSquadCharacter>(pRayHitCharacter)->GetbIsHighLight() != true)
							Cast<APlayerSquadCharacter>(pRayHitCharacter)->SetHighLight(true);
						}

					}
					pRayHitCharacter = nullptr;
				}
			}
			if (pRayHitEnemyCharacter != nullptr) {
				if (pRayHitEnemyCharacter->GetbIsHighLight() != false)
				pRayHitEnemyCharacter->SetHighLight(false);
				pRayHitEnemyCharacter = nullptr;
			}
		}
	}
}

void ASquadController::SetTargetCharacter()
{
	if(gameIns->IsBattleStart == true) {
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitTarget);
		if (HitTarget.bBlockingHit == true && BattleController->WhosTurn == false)
		{
			if (HitTarget.Actor == Cast<ASquadCharacter>(HitTarget.Actor))
			{
				if (BattleController != nullptr && BattleController->IsBattleStart == true) // 전투가 시작했을시
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
							ASquadGameMode* gameMode = SquadGameMode;
							gameMode->ClearCharacterInfoWidgetText_Right();

							if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Stay)
							{
								if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
								{
									Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetStay();
								}				
						
							}

							BattleController->SetDisableSkillTargeting(true);
							BattleController->SetDisableSkillTargeting(false);
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetHighLight(true);
							
							BattleController->SetSelectedCharacter(Cast<ASquadCharacter>(HitTarget.Actor)); // 인스턴스의 SelectedCharacter 초기화 함수
							Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Init_SkillButtonColor();


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

								SquadGameMode->UpDateWidgetText(Cast<ASquadCharacter>(HitTarget.Actor)); // 위젯을  타겟캐릭터 정보로 초기화
								SquadGameMode->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter));
								Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);

								pRayHitSelectedCharacter = Cast<ASquadCharacter>(HitTarget.Actor); 
								Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
								//Cast<ASquadCharacter>(HitTarget.Actor)->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
						
							}
							else if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_End) {

								SquadGameMode->UpDateWidgetText(Cast<ASquadCharacter>(HitTarget.Actor)); // 위젯을  타겟캐릭터 정보로 초기화
								SquadGameMode->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter));
								Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(true);

								pRayHitSelectedCharacter = Cast<ASquadCharacter>(HitTarget.Actor);
								Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
								/*
								SquadGameMode->ClearCharacterInfoWidgetText();
								Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Set_BattleWidgetSkilliconOpacity(false);
								*/
							}
						}

					}
				}
			}
			else // 캐릭터가 아닌 다른 오브젝트를 선택했을시
			{
				/* 기획 변경 삭제 예정
				auto gameIns = gameIns;

			
				if (Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter) != nullptr) // 인스턴스에 SelectedCharacter가 없다면
				{
				


				}
				else if (gameIns->BCIns != nullptr && gameIns->BCIns->IsBattleStart == true)
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
	if (gameIns->IsBattleStart == false) {
		GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitTarget);
		if (HitTarget.bBlockingHit == true)
		{
			if (HitTarget.Actor == Cast<APlayerSquadCharacter>(HitTarget.Actor))
			{
				if (Target_Explorer != nullptr) { // 타겟이 있을때
				
					//preTarget_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(0.5f);
					preTarget_Explorer = Target_Explorer;
					preTarget_Explorer->SetHighLight(false);
					
					Target_Explorer = Cast<APlayerSquadCharacter>(HitTarget.Actor);
					//Target_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
					Target_Explorer->SetHighLight(true);
					BattleController->SetSelectedCharacter(Target_Explorer);

					Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Init_SkillButtonColor();

					//preTarget_Explorer = Target_Explorer;
				}
				else{ // 타겟이 없을때
									
					Target_Explorer = Cast<APlayerSquadCharacter>(HitTarget.Actor);
					Target_Explorer->SetHighLight(true);
					BattleController->SetSelectedCharacter(Target_Explorer);

					//Target_Explorer->GetStatustBarWidget()->SetBarRenderOpacity(1.f);
					//preTarget_Explorer = Target_Explorer;
					Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Init_SkillButtonColor();

				}	

				Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();
				//Cast<APlayerSquadCharacter>(HitTarget.Actor)->DebugMessage_CharacterState();
			}


		}
		SetSelectedCharacterInfo();
	}
}

int32 ASquadController::SwitchGamePlayValue()
{
	if (IsGamePlay == 0) IsGamePlay = 1;
	else if (IsGamePlay == 1) IsGamePlay = 0;

	return IsGamePlay;
}




void ASquadController::SetKeyBindSkillButton1() // 사격버튼
{
	if (1)
	{
		
		if (AActor* SC = BattleController->GetSelectedCharacter())
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
	if (1)	{
	
		if (AActor* SC = BattleController->GetSelectedCharacter())	{
			Cast<APlayerSquadCharacter>(SC)->SetSkill1();
		}

	}

}

void ASquadController::SetKeyBindSkillButton3() // 스킬2버튼
{
	if (1) {
		if (AActor* SC = BattleController->GetSelectedCharacter()) {
		
			Cast<APlayerSquadCharacter>(SC)->SetSkill2();
		}
	}
}

void ASquadController::SetKeyBindSkillButton4() // 장전버튼
{
	if (1)	{
		
		if (AActor* SC = BattleController->GetSelectedCharacter()) {
			
			Cast<APlayerSquadCharacter>(SC)->SetReloadReady();
		}	
			
	}

}

void ASquadController::SetKeyBindSkillButton5() // 엄폐버튼
{
	if (1)
	{

		if (AActor* SC = BattleController->GetSelectedCharacter()) {
			
			Cast<APlayerSquadCharacter>(SC)->SetCoverReady();
		}

	}

}



void ASquadController::SetkeyBindMousewheel()
{
	if (BattleController->IsBattleStart) {
		if (AActor* SC = BattleController->GetSelectedCharacter()) {
		
			BattleController->ReverseControlChangeSelectedCharacter();
			Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter())->PlaySelectedSound();
		}
	}
	else {
		ControlTarget_ExplorerWheel();
	}
	//UGameplayStatics::PlaySound2D(this, Selected_Sound);
}

void ASquadController::SetkeyBindMousewheelreverse()
{
	if (BattleController->IsBattleStart) {
		if (AActor* SC = BattleController->GetSelectedCharacter()) {
			
			BattleController->ControlChangeSelectedCharacter();
			Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter())->PlaySelectedSound();
		}
	}
	else {
		ControlTarget_ExplorerWheelReverse();
	}
	//UGameplayStatics::PlaySound2D(this, Selected_Sound);
}

void ASquadController::ControlTarget_ExplorerWheel()
{
	if(Target_Explorer) {
		auto list = gameIns->SCMIns->FriendlyCharList;
		int32 tempNum;
		for (int i = 0; i < list.Num(); i++) {
			if (Cast<APlayerSquadCharacter>(list[i]) == Target_Explorer) { tempNum = i + 1; }
			if (tempNum == list.Num()) { tempNum = 0; }
		}

		Cast<APlayerSquadCharacter>(list[tempNum])->SetHighLight(true);
		preTarget_Explorer = Target_Explorer;
		preTarget_Explorer->SetHighLight(false);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[tempNum]);
		BattleController->SetSelectedCharacter(Target_Explorer);
	}
	else {
		auto list = gameIns->SCMIns->FriendlyCharList;
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[0]);		
		BattleController->SetSelectedCharacter(Target_Explorer);
	}

	Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Init_SkillButtonColor();
	Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter())->PlaySelectedSound();
	SetSelectedCharacterInfo();
}

void ASquadController::ControlTarget_ExplorerWheelReverse()
{
	if (Target_Explorer) {
		auto list = gameIns->SCMIns->FriendlyCharList;
		int32 tempNum;
		for (int i = 0; i < list.Num(); i++) {
			if (Cast<APlayerSquadCharacter>(list[i]) == Target_Explorer) { tempNum = i - 1; }
			if (tempNum == -1 ) { tempNum = list.Num() - 1; }
		}

		Cast<APlayerSquadCharacter>(list[tempNum])->SetHighLight(true);
		preTarget_Explorer = Target_Explorer;
		preTarget_Explorer->SetHighLight(false);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[tempNum]);
		BattleController->SetSelectedCharacter(Target_Explorer);
	}
	else {
		auto list = gameIns->SCMIns->FriendlyCharList;
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		Target_Explorer = Cast<APlayerSquadCharacter>(list[0]);
		BattleController->SetSelectedCharacter(Target_Explorer);
	}

	Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->Init_SkillButtonColor();
	Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter())->PlaySelectedSound();
	SetSelectedCharacterInfo();
}

// 

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
	auto list = gameIns->SCMIns->FriendlyCharList;

	if (SquadGameMode->GetCurrentWidget() == Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget()))
		Cast<UBattleWidget>(SquadGameMode->GetCurrentWidget())->ClearWidget_SkillPart();

	if(BattleController->GetSelectedCharacter() == nullptr) {
		BattleController->SetSelectedCharacter(Cast<APlayerSquadCharacter>(list[0]));
		Cast<APlayerSquadCharacter>(list[0])->SetHighLight(true);
		SquadGameMode->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(list[0]));
		SquadGameMode->UpDateWidgetText(Cast<APlayerSquadCharacter>(list[0]));
	}
	else {
		APlayerSquadCharacter* PC = Cast<APlayerSquadCharacter>(BattleController->GetSelectedCharacter());
		SquadGameMode->UpDateWidgetText_Right2(Cast<APlayerSquadCharacter>(PC));
		SquadGameMode->UpDateWidgetText(Cast<APlayerSquadCharacter>(PC));
	
	}

	SquadGameMode->SetBattleWidgetSkillButtonOpacity_transparent();

}

void ASquadController::SetMenuInput(bool bIsStop)
{
	auto SCM = Cast<ASquadCameraManager>(gameIns->SCMIns);
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

void ASquadController::SetBattleController()
{
	BattleController = gameIns->BCIns;
}


// Key Bind & UnBind 하드코딩 스타일의 언바운드 & 바운드 보존용
/*
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
*/