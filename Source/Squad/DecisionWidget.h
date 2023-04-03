// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CommonStruct.h"
#include "DecisionWidget.generated.h"



USTRUCT(Atomic, BlueprintType)
struct FEventResult
{
	GENERATED_BODY()

public:
	//APlayerSquadCharacter* CharacterInfo;
	EDecisionEventResultEnum FinalResult;
	int8 ResultValue;
	// 1 = Succ , 2 = Fail
	int32 EventSuccFail = 0;
};


/**
 * 
 */
UCLASS()
class SQUAD_API UDecisionWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickEvent1();

	UFUNCTION()
	void OnClickEvent2();

	//////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UButton* Event1;

	UPROPERTY(meta = (BindWidget))
		class UButton* Event2;

	UPROPERTY(meta = (BindWidget))
		class UButton* Event3;

	/////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* EventName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event1Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event2Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event3Name;

	

	FString InitName = TEXT("Fight");

	/////////////////////////////////////////////

	
	UFUNCTION()
		void SetDecisionState(EDecisionEnum EWidgetState);

	UFUNCTION()
		void EventResult_Damage(bool AllUnits, float DamageValue);
	UFUNCTION()
		void EventResult_Recover(bool AllUnits, float RecoverValue);


	UFUNCTION()
		void EventResult_Battle();
	void EventResult_BossBattle();
	UFUNCTION()
		void EventResult_BattleAvoidence();

	void EventResult_IncounterEnd();

	void EventResult_Default();

	UFUNCTION()
		int32 BattleEventWeighRand();

	UFUNCTION()
		int32 NonBattleEventWeighRand();

	EDecisionEnum WidgetState;
	int32 MainDecisionStream;
	int32 SubDecisionStream;
	int32 SubDecisionStreamSuccFail = 0;
	FText EventResultText;
	FText EventResultValueText;

	UFUNCTION()
		void DecisionResult();

	void DecisionBossResult();

	UFUNCTION()
		void SetTrapResult();

	UFUNCTION()
		void SetTrapButton_1();

	UFUNCTION()
		void SetTrapButton_2();


	int32 SetButtonValue(int32 mainEventStreamNumber);

	void SetButtonValue_Boss();

	void SetButton(int8 ButtonNum, float ProbSucc, EDecisionEventResultEnum SuccResultData, int8 SuccResultValue, EDecisionEventResultEnum FailResultData, int8 FailResultValue);

	UFUNCTION()
	void ResultButton_1();
	UFUNCTION()
	void ResultButton_2();
	UFUNCTION()
	void ResultButton_3();

	UFUNCTION()
	void SetResultButton(uint8 buttonNum);

	FEventResult Button1_Result;
	FEventResult Button2_Result;
	FEventResult Button3_Result;

	EDecisionEventResultEnum FinalEvent;
	uint8 FinalEventValue;

	UFUNCTION()
		void Set_EventText(int32 mainStreamNum);

	UFUNCTION()
		void SetText_EventNonBattle();




	UFUNCTION()
		void SetBattleButton_1();

	UFUNCTION()
		void SetBattleButton_2();
	
	UFUNCTION()
		void SetSupplyButton_1();

	UFUNCTION()
		void SetSupplyButton_2();

public:

	UFUNCTION()
		void SetResult(bool EventState);
	UFUNCTION()
	void SetBossResult();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEXT")
		FText Message = NSLOCTEXT("UserMessage", "Messge", " Culture ");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEXT")
		UTextBlock* TestTextBlock;


};
