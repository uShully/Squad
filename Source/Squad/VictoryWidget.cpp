// Fill out your copyright notice in the Description page of Project Settings.

#include "VictoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SquadGameInstance.h"
#include "BattleController.h"

void UVictoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultButton = Cast<UButton>(GetWidgetFromName("RESULTBUTTON"));
	if (ResultButton != nullptr)
	{
		ResultButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnClickResultButton);
	}

	ResultTextBlock = Cast<UTextBlock>(GetWidgetFromName("RESULTTEXTBLOCK"));
	if (ResultTextBlock != nullptr)
	{
		ResultTextBlock->SetText(FText::FromString(TEXT("Victory")));
	}
}

void UVictoryWidget::OnClickResultButton()
{
	auto BCins = Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns);
	BCins->ResultBattle();


}