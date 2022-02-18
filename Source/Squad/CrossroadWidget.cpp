// Fill out your copyright notice in the Description page of Project Settings.

#include "CrossroadWidget.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "BattleController.h"
#include "SquadGameInstance.h"

void UCrossroadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Crossroad = Cast<UButton>(GetWidgetFromName("CROSSROADBUTTON"));
	if (Crossroad != nullptr)
	{
		Crossroad->OnClicked.AddDynamic(this, &UCrossroadWidget::OnClickEvent1);
	}
	Home = Cast<UButton>(GetWidgetFromName("HOMEBUTTON"));
	if (Home != nullptr)
	{
		Home->OnClicked.AddDynamic(this, &UCrossroadWidget::OnClickEvent2);
	}


	CrossroadName = Cast<UTextBlock>(GetWidgetFromName("CROSSROADNAME"));
	if (CrossroadName != nullptr)
	{
		CrossroadName->SetText(FText::FromString(CrossRoadStringName));
	}

	HomeName = Cast<UTextBlock>(GetWidgetFromName("HOMENAME"));
	if (HomeName != nullptr)
	{
		HomeName->SetText(FText::FromString(HomeStringName));
	}
}

void UCrossroadWidget::OnClickEvent1() // ÀüÅõ
{


}

void UCrossroadWidget::OnClickEvent2() // µµ¸Á
{
	

	//gameMode->BTIns = nullptr;
}


