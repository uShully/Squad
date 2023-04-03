// Fill out your copyright notice in the Description page of Project Settings.

#include "DecisionWidget.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "BattleController.h"
#include "SquadGameInstance.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Text.h"
#include "Internationalization/TextLocalizationManager.h"
#include "Internationalization/StringTableRegistry.h"
#include "Engine/Engine.h"

void UDecisionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Event1 = Cast<UButton>(GetWidgetFromName("EVENT1"));
	Event2 = Cast<UButton>(GetWidgetFromName("EVENT2"));
	Event3 = Cast<UButton>(GetWidgetFromName("EVENT3"));
	Event1Name = Cast<UTextBlock>(GetWidgetFromName("EVENT1NAME"));
	Event2Name = Cast<UTextBlock>(GetWidgetFromName("EVENT2NAME"));
	Event3Name = Cast<UTextBlock>(GetWidgetFromName("EVENT3NAME"));
	
	if (Event1 != nullptr)
	{
		Event1->OnClicked.AddDynamic(this, &UDecisionWidget::ResultButton_1);
	}

	if (Event2 != nullptr)
	{
		Event2->OnClicked.AddDynamic(this, &UDecisionWidget::ResultButton_2);
	}

	if (Event3 != nullptr)
	{
		Event3->OnClicked.AddDynamic(this, &UDecisionWidget::ResultButton_3);
	}

	//FStringTableRegistry::Get().UnregisterStringTable("MyStrings");

	//LOCTABLE_FROMFILE_GAME("TranslateData", "GoodbyeWorld", "/Game/DATATABLE/TranslateData.TranslateData");
	
	
}

void UDecisionWidget::OnClickEvent1() // 전투
{
	FCultureRef cultureRef = FInternationalization::Get().GetCurrentLanguage();
	FCulture cultureSetting =  cultureRef.Get();
	
}

void UDecisionWidget::OnClickEvent2() // 도망
{

	/* 도망 트리거
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns)->Control_ResultToRun();
	Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();
	
	RemoveFromParent();
	*/
	//gameMode->BTIns = nullptr;
}

void UDecisionWidget::SetResult(bool EventState) // BattleTrigger - SetEvent  전투 비전투 확률 조작
{

	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(false);

	if (EventState == 0)
	{
		WidgetState = EDecisionEnum::NonBattle; // nonBattleEvent
	
	}
	else if (EventState == 1)
	{
		WidgetState = EDecisionEnum::Battle; // BattleEvent

	}


	SetDecisionState(WidgetState);
}

void UDecisionWidget::SetBossResult() // BattleTrigger - SetEvent  전투 비전투 확률 조작
{
	WidgetState = EDecisionEnum::Boss;
	   
	SetDecisionState(WidgetState);
}

void UDecisionWidget::SetDecisionState(EDecisionEnum EWidgetState)
{	

	if (WidgetState == EDecisionEnum::Battle)
	{		
		int32 mainstreamNum = SetButtonValue(BattleEventWeighRand());
		Set_EventText(mainstreamNum);
		MainDecisionStream = mainstreamNum;
	}
	else if (WidgetState == EDecisionEnum::NonBattle)
	{				
		int32 mainstreamNum = SetButtonValue(NonBattleEventWeighRand());
		Set_EventText(mainstreamNum);
		MainDecisionStream = mainstreamNum;
	}
	else if (WidgetState == EDecisionEnum::Boss)
	{
		SetButtonValue_Boss();
	}
	
}

int32 UDecisionWidget::BattleEventWeighRand()
{
	TMap<int32, float> FinalMap;
	TMap < int32, float > a;

	int32 TotalProb = 0;

	for (int i = 0; i < 6; i++) {
		FEventProb* pBEventProb = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetBattleEventProbData(i);
		a.Add(pBEventProb->EventNum, pBEventProb->EventProb);
	}	

	for (auto& b : a) {
		TotalProb += b.Value;
	}

	for (auto& c : a) {
		float Prob = c.Value / TotalProb;		
		FinalMap.Add(c.Key, Prob);		
	}

	float randPivot = FMath::RandRange(0.f, 1.f);


	double acc = 0;

	for (auto& d : FinalMap) {
		acc += d.Value;

		if (randPivot <= acc) {
			return d.Key;
		}
	}
	
	return NULL;
}

int32 UDecisionWidget::NonBattleEventWeighRand() // 비전투 이벤트 
{
	TMap<int32, float> FinalMap;
	TMap < int32, float > a;

	int32 TotalProb = 0;
	
	for (int i = 6; i <= 14; i++) {
		FEventProb* pBEventProb = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetNonBattleEventProbData(i);		
		a.Add(pBEventProb->EventNum, pBEventProb->EventProb);
	}
	
	for (auto& b : a) {
		TotalProb += b.Value;
	}

	for (auto& c : a) {
		float Prob = c.Value / TotalProb;
		FinalMap.Add(c.Key, Prob);
	}

	float randPivot = FMath::RandRange(0.f, 1.f);
	

	double acc = 0;

	for (auto& d : FinalMap) {
		acc += d.Value;

		if (randPivot <= acc) {
			
			return d.Key;
		}
	}
	

	return NULL;
}



int32 UDecisionWidget::SetButtonValue(int32 mainEventStreamNumber) 
{	
	struct FEventValue* pEventValue = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->GetEventValueData(mainEventStreamNumber - 1);

	
	if (pEventValue->EventType == EDecisionEnum::Default) {
		OnClickEvent1();
		auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
		RemoveFromParent();

		return NULL;
	}
	else {
		auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(false);

		// 첫번째 선택지
		SetButton(1, pEventValue->FirstOptionProbability, pEventValue->FirstOptionSuccResult, pEventValue->FirstOptionSuccResultValue, pEventValue->FirstOptionFailResult, pEventValue->FirstOptionFailResultValue);
		// 두번째 선택지 
		SetButton(2, pEventValue->SecondOptionProbability, pEventValue->SecondOptionSuccResult, pEventValue->SecondOptionSuccResultValue, pEventValue->SecondOptionFailResult, pEventValue->SecondOptionFailResultValue);
		// 세번째 선택지
		SetButton(3, pEventValue->ThirdOptionProbability, pEventValue->ThirdOptionSuccResult, pEventValue->ThirdOptionSuccResultValue, pEventValue->ThirdOptionFailResult, pEventValue->ThirdOptionFailResultValue);
	
		return mainEventStreamNumber;
	}
}

void UDecisionWidget::SetButtonValue_Boss()
{
	
		auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(false);

		// 첫번째 선택지
		SetButton(1, 100, EDecisionEventResultEnum::BossBattleStart, 0, EDecisionEventResultEnum::BossBattleStart, 0);
		// 두번째 선택지 
		SetButton(2, 0, EDecisionEventResultEnum::Default, 0, EDecisionEventResultEnum::Default, 0);
		// 세번째 선택지
		SetButton(3, 0, EDecisionEventResultEnum::Default, 0, EDecisionEventResultEnum::Default,0);

	
}

void UDecisionWidget::SetButton(int8 ButtonNum, float ProbSucc, EDecisionEventResultEnum SuccResultData, int8 SuccResultValue, EDecisionEventResultEnum FailResultData, int8 FailResultValue)
{
	// 랜덤함수 성공 실패 결정
	bool SuccOrFail = false;
	float ProbabilitySucc = ProbSucc;
	float rand = FMath::RandRange(0.f, 100.f);
	if (ProbabilitySucc == 0) {
		SuccOrFail = false;
	}
	else{
	if (rand <= ProbabilitySucc)
		SuccOrFail = true;
	else
		SuccOrFail = false;
	}

	switch(ButtonNum) {

	case 1:
		if (SuccOrFail == true) {

			Button1_Result.FinalResult = SuccResultData;
			Button1_Result.ResultValue = SuccResultValue;
			Button1_Result.EventSuccFail = 1;
		}
		else { // 실패
			Button1_Result.FinalResult = FailResultData;
			Button1_Result.ResultValue = FailResultValue;
			Button1_Result.EventSuccFail = 2;
		}
		break;
	case 2:
		if (SuccOrFail == true) {

			Button2_Result.FinalResult = SuccResultData;
			Button2_Result.ResultValue = SuccResultValue;
			Button2_Result.EventSuccFail = 1;
		}
		else { // 실패
			Button2_Result.FinalResult = FailResultData;
			Button2_Result.ResultValue = FailResultValue;
			Button2_Result.EventSuccFail = 2;
		}
		break;
	case 3:
		if (SuccOrFail == true) {

			Button3_Result.FinalResult = SuccResultData;
			Button3_Result.ResultValue = SuccResultValue;
			Button3_Result.EventSuccFail = 1;
		}
		else { // 실패
			Button3_Result.FinalResult = FailResultData;
			Button3_Result.ResultValue = FailResultValue;
			Button3_Result.EventSuccFail = 2;
		}
		break;
	}

}

void UDecisionWidget::ResultButton_1()
{
	SetResultButton(1);
}
void UDecisionWidget::ResultButton_2()
{
	SetResultButton(2);
}
void UDecisionWidget::ResultButton_3()
{
	SetResultButton(3);
}

void UDecisionWidget::SetResultButton(uint8 buttonNum)
{
	switch(buttonNum) {
	case 1:
		FinalEvent = Button1_Result.FinalResult;
		FinalEventValue = Button1_Result.ResultValue;
		SubDecisionStream = 1;
		SubDecisionStreamSuccFail = Button1_Result.EventSuccFail;
		break;
	case 2:
		FinalEvent = Button2_Result.FinalResult;
		FinalEventValue = Button2_Result.ResultValue;
		SubDecisionStreamSuccFail = Button2_Result.EventSuccFail;
		SubDecisionStream = 2;
		break;
	case 3:
		FinalEvent = Button3_Result.FinalResult;
		FinalEventValue = Button3_Result.ResultValue;
		SubDecisionStream = 3;
		SubDecisionStreamSuccFail = Button3_Result.EventSuccFail;
		break;
	}
		
	    
	if (FinalEvent == EDecisionEventResultEnum::BattleStart) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "0");
		EventResultValueText = FText::FromString(TEXT(" "));
		EventResult_Battle();
	}
	else if (FinalEvent == EDecisionEventResultEnum::BossBattleStart){
		EventResult_BossBattle();
	}
	else if (FinalEvent == EDecisionEventResultEnum::BattleAvoidance) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "1");
		EventResultValueText = FText::FromString(TEXT(" "));
		EventResult_BattleAvoidence();
	}
	else if (FinalEvent == EDecisionEventResultEnum::AllUnitDamage) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "2");
		EventResultValueText = FText::FromString(FString::FromInt(FinalEventValue));
		EventResult_Damage(true, FinalEventValue);
	}
	else if (FinalEvent == EDecisionEventResultEnum::TargetUnitDamage) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "3");
		EventResultValueText = FText::FromString(FString::FromInt(FinalEventValue));
		EventResult_Damage(true, FinalEventValue);
	}
	else if (FinalEvent == EDecisionEventResultEnum::AllUnitRecover) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "4");
		EventResultValueText = FText::FromString(FString::FromInt(FinalEventValue));
		EventResult_Recover(true, FinalEventValue);
	}
	else if (FinalEvent == EDecisionEventResultEnum::TargetUnitRecover) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "5");
		EventResultValueText = FText::FromString(FString::FromInt(FinalEventValue));
		EventResult_Recover(true, FinalEventValue);
	}
	else if (FinalEvent == EDecisionEventResultEnum::IncounterEnd) {
		EventResultText = FText::FromStringTable("/Game/DATATABLE/EventResultTranslateData.EventResultTranslateData", "6");
		EventResultValueText = FText::FromString(TEXT(" "));
		EventResult_IncounterEnd();
	}
	else if (FinalEvent == EDecisionEventResultEnum::Default) {
		//EventResult_Default();
	}
}

/////////////////////////////////////////
////		    텍스트 파트		     ////
/////////////////////////////////////////


void UDecisionWidget::Set_EventText(int32 mainStreamNum)
{
	if (WidgetState == EDecisionEnum::Battle)
	{
		FText evnetName = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 0, 0));
		FText eventContent = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 0, 1));

		FText option1_Name = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 1, 0));
		FText option2_Name = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 2, 0));
		FText option3_Name = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 3, 0));

		EventName->SetText(evnetName);
		Event->SetText(eventContent);

		Event1Name->SetText(option1_Name);
		Event2Name->SetText(option2_Name);
		Event3Name->SetText(option3_Name);
	}
	else if (WidgetState == EDecisionEnum::NonBattle)
	{
		FText evnetName = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 0, 0));
		FText eventContent = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 0, 1));

		FText option1_Name = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 1, 0));
		FText option2_Name = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 2, 0));
		FText option3_Name = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), mainStreamNum, 3, 0));

		EventName->SetText(evnetName);
		Event->SetText(eventContent);

		Event1Name->SetText(option1_Name);
		Event2Name->SetText(option2_Name);
		Event3Name->SetText(option3_Name);
	}	
}

void UDecisionWidget::SetText_EventNonBattle()
{
	if (Event1Name != nullptr)
	{
		EventName->SetText(FText::FromString(TEXT("비전투")));
		//Event->SetText(FText::FromString(TEXT("이벤트 내용")));

		//auto text = FText::FromStringTable("TranslateData", "GoodbyeWorld");
		auto text = FText::FromStringTable("/Game/DATATABLE/TranslateData.TranslateData", "GoodbyeWorld");
		Event->SetText(text);
		Event1Name->SetText(FText::FromString(TEXT("옵션1")));
		Event2Name->SetText(FText::FromString(TEXT("옵션2")));
		Event2Name->SetText(FText::FromString(TEXT("옵션3")));
	}
}


void UDecisionWidget::DecisionResult()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	gameMode->ViewDecisionResultWidget(FinalEvent, MainDecisionStream, SubDecisionStream, SubDecisionStreamSuccFail, EventResultText , EventResultValueText);
	// mainDecisionStream == 이벤트 번호
	// SubDecisionStream == 

}

void UDecisionWidget::DecisionBossResult()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());

	gameMode->ViewDecisionBossResultWidget(FinalEvent, MainDecisionStream, SubDecisionStream, SubDecisionStreamSuccFail, EventResultText, EventResultValueText);
	// mainDecisionStream == 이벤트 번호
	// SubDecisionStream == 

}

////////////////////////////////////////

void UDecisionWidget::EventResult_Battle()
{
	// 전투 트리거


	DecisionResult();
	RemoveFromParent();

	//auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//gameMode->StartBattle();
}

void UDecisionWidget::EventResult_BossBattle()
{
	// 전투 트리거


	DecisionBossResult();
	RemoveFromParent();

	//auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//gameMode->StartBattle();
}

void UDecisionWidget::EventResult_BattleAvoidence()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();

	DecisionResult();
	RemoveFromParent();
}

void UDecisionWidget::EventResult_IncounterEnd()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);

	if (WidgetState == EDecisionEnum::Battle)
		Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();

	DecisionResult();
	RemoveFromParent();
}

void UDecisionWidget::EventResult_Default()
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);

	DecisionResult();
	RemoveFromParent();
}

void UDecisionWidget::EventResult_Damage(bool AllUnits, float DamageValue)
{
	TArray<AActor*> Charlist = Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns)->FriendlyCharList;

	if (AllUnits == true)
	{
		for (int32 i = 0; i < Charlist.Num(); i++)
		{
			UGameplayStatics::ApplyDamage(Cast<APlayerSquadCharacter>(Charlist[i]), DamageValue , GetWorld()->GetFirstPlayerController(), nullptr, nullptr);
		}		
	}
	else
	{
		AActor* SelectedChar = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter;
		UGameplayStatics::ApplyDamage(Cast<APlayerSquadCharacter>(SelectedChar), DamageValue, GetWorld()->GetFirstPlayerController(), nullptr, nullptr);
	}	

	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	if (WidgetState == EDecisionEnum::Battle)
		Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();


	DecisionResult();
	RemoveFromParent();
}

void UDecisionWidget::EventResult_Recover(bool AllUnits, float RecoverValue)
{
	TArray<AActor*> Charlist = Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns)->FriendlyCharList;
	if(AllUnits == true) {	
		for (int32 i = 0; i < Charlist.Num(); i++)
		{
			//if(Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint <= Cast<APlayerSquadCharacter>(Charlist[i])->MaxLifePoint)
			Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint += RecoverValue;
			if (Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint > Cast<APlayerSquadCharacter>(Charlist[i])->MaxLifePoint)
				Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint = Cast<APlayerSquadCharacter>(Charlist[i])->MaxLifePoint;
			// 캐릭터 생명력 최대수치보다 높으면 안됨
		}
	}
	else {
		AActor* SelectedChar = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SelectedCharacter;
		Cast<APlayerSquadCharacter>(SelectedChar)->LifePoint += RecoverValue;
		if (Cast<APlayerSquadCharacter>(SelectedChar)->LifePoint > Cast<APlayerSquadCharacter>(SelectedChar)->MaxLifePoint)
			Cast<APlayerSquadCharacter>(SelectedChar)->LifePoint = Cast<APlayerSquadCharacter>(SelectedChar)->MaxLifePoint;
	}	

	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	if (WidgetState == EDecisionEnum::Battle)
		Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();

	DecisionResult();
	RemoveFromParent();
}



// 정리 후 삭제 //

/////////////////////////////////////////
////		    함정 파트		     ////
/////////////////////////////////////////


void UDecisionWidget::SetTrapResult()
{
	if (Event1Name != nullptr)
	{
		EventName->SetText(FText::FromString(TEXT("함정")));
		Event->SetText(FText::FromString(TEXT("지뢰를 밟음")));

		Event1Name->SetText(FText::FromString(TEXT("해체")));
		Event2Name->SetText(FText::FromString(TEXT("도주")));
	}

	if (Event1 != nullptr)
	{
		Event1->OnClicked.AddDynamic(this, &UDecisionWidget::SetTrapButton_1);
	}

	if (Event2 != nullptr)
	{
		Event2->OnClicked.AddDynamic(this, &UDecisionWidget::SetTrapButton_2);
	}
}

void UDecisionWidget::SetTrapButton_1()
{
	MainDecisionStream = 1;

	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal < 50.f)
	{

		UE_LOG(LogClass, Log, L" 1 - 1 ");
		SubDecisionStream = 1;
	//	SetNonBattleProcess();
	}
	else if (randVal >= 50.f && randVal <= 100.f)
	{
		// 데미지 코드
		//SetPlayerCharacterDamageControl(true, 5, 0);

		UE_LOG(LogClass, Log, L" 1 - 2 ");
		SubDecisionStream = 2;
	//	SetNonBattleProcess();
	}
}

void UDecisionWidget::SetTrapButton_2()
{
	MainDecisionStream = 2;
	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal < 75.f)
	{
		// 데미지
		//SetPlayerCharacterDamageControl(true, 5, 0);

		UE_LOG(LogClass, Log, L" 2 - 1 ");
		SubDecisionStream = 1;
		//SetNonBattleProcess();
	}
	else if (randVal >= 75.f && randVal <= 100.f)
	{

		UE_LOG(LogClass, Log, L" 2 - 2 ");
		SubDecisionStream = 2;
		//SetNonBattleProcess();
	}

	/*
	// 데미지 코드
	SetPlayerCharacterDamageControl(false, 1, 0);

	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);



	Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();

	DecisionResult();
	RemoveFromParent();
	*/
}

///////////////////////////////////////



void UDecisionWidget::SetBattleButton_1()
{
	MainDecisionStream = 1;

	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal <= 100.f)
	{
		UE_LOG(LogClass, Log, L" 1 - 1 ");
		SubDecisionStream = 1;
		EventResult_Battle();
	}
	else if (false)
	{
		UE_LOG(LogClass, Log, L" 1 - 2 ");
		SubDecisionStream = 2;
		EventResult_Battle();
	}
}

void UDecisionWidget::SetBattleButton_2()
{
	MainDecisionStream = 2;
	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal < 50.f)
	{
		UE_LOG(LogClass, Log, L" 2 - 1 ");
		SubDecisionStream = 1;
		EventResult_Battle();
	}
	else if (randVal >= 50.f && randVal <= 100.f)
	{
		UE_LOG(LogClass, Log, L" 2 - 2 ");
		SubDecisionStream = 2;
		EventResult_Battle();
	}
}

/////////////////////////////////////////
////		    보급 파트		     ////
/////////////////////////////////////////



void UDecisionWidget::SetSupplyButton_1()
{
	MainDecisionStream = 1;

	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal < 75.f)
	{
		// 데미지 & 회복 코드
		//SetPlayerCharacterDamageControl(true, 0, 5);

		UE_LOG(LogClass, Log, L" 1 - 1 ");
		SubDecisionStream = 1;
		//SetNonBattleProcess();
	}
	else if (randVal >= 75.f && randVal <= 100.f)
	{
		UE_LOG(LogClass, Log, L" 1 - 2 ");
		SubDecisionStream = 2;
		//SetNonBattleProcess();
	}
}

void UDecisionWidget::SetSupplyButton_2()
{
	MainDecisionStream = 2;
	auto randVal = FMath::RandRange(0.f, 100.f);

	if (randVal >= 0 && randVal <= 100.f)
	{
		UE_LOG(LogClass, Log, L" 2 - 1 ");
		SubDecisionStream = 1;
		//SetNonBattleProcess();
	}
	else if (false)
	{
		UE_LOG(LogClass, Log, L" 2 - 2 ");
		SubDecisionStream = 2;
		//SetNonBattleProcess();
	}
}

/////////////////////////////////////////