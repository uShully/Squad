// Fill out your copyright notice in the Description page of Project Settings.

#include "DecisionResultWidget.h"
#include "SquadGameMode.h"
#include "SquadGameInstance.h"
#include "Engine/Engine.h"

void UDecisionResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmationButton = Cast<UButton>(GetWidgetFromName("CONFIRMATIONBUTTON"));
	Eventname = Cast<UTextBlock>(GetWidgetFromName("EVENTNAME"));
	Event = Cast<UTextBlock>(GetWidgetFromName("EVENT"));
	OutputValue = Cast<UTextBlock>(GetWidgetFromName("OUTPUTVALUE"));
	
	SetResultText();

	

	if (ConfirmationButton != nullptr)
	{
		ConfirmationButton->OnClicked.AddDynamic(this, &UDecisionResultWidget::OnClickConfirmationButton);
	}

	RemoveFromParent();
}

void UDecisionResultWidget::OnClickConfirmationButton()
{
	if (DecisionResultEnum == EDecisionEventResultEnum::BattleStart || DecisionResultEnum == EDecisionEventResultEnum::BossBattleStart)
	{
		auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
		gameMode->StartBattle();
	}
	else
	{
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	}

	
	RemoveFromParent();
}

void UDecisionResultWidget::SetResultText()
{
	if( MainStream >= 1 && MainStream <= 6) {
	FText evnetName = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), MainStream, 0, 0));
	FText eventContent = FText::FromStringTable("/Game/DATATABLE/BattleEventTranslateData.BattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), MainStream, SubStream, SubStreamSuccFail));
	
	Eventname->SetText(evnetName);
	Event->SetText(eventContent);
#define LOCTEXT_NAMESPACE "MyNamespace"
	auto text = FText::Format(LOCTEXT("ExampleFText", "{EventResultText} {EventResultValueText}"), EventResultText, EventResultValueText);
	OutputValue->SetText(text);
#undef LOCTEXT_NAMESPACE

	
	}
	else if (MainStream == 999) {

	}
	else {
		FText evnetName = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), MainStream, 0, 0));
		FText eventContent = FText::FromStringTable("/Game/DATATABLE/NonBattleEventTranslateData.NonBattleEventTranslateData", FString::Printf(TEXT("%d%d%d"), MainStream, SubStream, SubStreamSuccFail));
		
		Eventname->SetText(evnetName);
		Event->SetText(eventContent);
#define LOCTEXT_NAMESPACE "MyNamespace"
		auto text = FText::Format(LOCTEXT("ExampleFText", " {EventResultText} {EventResultValueText}"), EventResultText, EventResultValueText);
		OutputValue->SetText(text);
#undef LOCTEXT_NAMESPACE
	
	}
}