// Fill out your copyright notice in the Description page of Project Settings.

#include "DecisionWidget.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "BattleController.h"
#include "SquadGameInstance.h"

void UDecisionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Event1 = Cast<UButton>(GetWidgetFromName("EVENT1"));
	if (Event1 != nullptr)
	{
		Event1->OnClicked.AddDynamic(this, &UDecisionWidget::OnClickEvent1);
	}
	Event2 = Cast<UButton>(GetWidgetFromName("EVENT2"));
	if (Event2 != nullptr)
	{
		Event2->OnClicked.AddDynamic(this, &UDecisionWidget::OnClickEvent2);
	}


	Event1Name = Cast<UTextBlock>(GetWidgetFromName("EVENT1NAME"));
	if(Event1Name != nullptr)
	{
		Event1Name->SetText(FText::FromString(InitName));
	}
}

void UDecisionWidget::OnClickEvent1() // ¿¸≈ı
{
	
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	gameMode->StartBattle();
	
}

void UDecisionWidget::OnClickEvent2() // µµ∏¡
{
	auto gameMode = Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode());
	Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);
	Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns)->Control_ResultToRun();
	Cast<ABattleTrigger>(gameMode->BTIns)->DeleteEnemyCharacter();
	
	RemoveFromParent();
	
	//gameMode->BTIns = nullptr;
}

