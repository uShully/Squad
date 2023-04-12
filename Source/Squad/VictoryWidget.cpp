// Fill out your copyright notice in the Description page of Project Settings.

#include "VictoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SquadGameInstance.h"
#include "SquadController.h"
#include "BattleController.h"
#include "Engine/Engine.h"

void UVictoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultButton = Cast<UButton>(GetWidgetFromName("RESULTBUTTON"));
	if (ResultButton != nullptr)
	{
		ResultButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnClickResultButton);
	}

	ResultTextBlock = Cast<UTextBlock>(GetWidgetFromName("RESULTTEXTBLOCK"));
}

void UVictoryWidget::OnClickResultButton()
{
	auto gameIns = GetWorld()->GetGameInstance();
	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(true);
	//Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement(false);

	//Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(true);

	auto SgameIns = Cast<USquadGameInstance>(gameIns);
	Cast<USquadGameInstance>(SgameIns)->SCMIns->UnitPos_Check_First = false;
	Cast<USquadGameInstance>(SgameIns)->SCMIns->UnitPos_Check_Last = false;
	Cast<USquadGameInstance>(SgameIns)->SCMIns->UnitPos_Init = false;
	Cast<USquadGameInstance>(SgameIns)->SCMIns->ControlValue_PlayerCharacterMovement_BeginPlay = true;
	Cast<USquadGameInstance>(SgameIns)->SCMIns->SetUnitMovement_Delay(3.8);

	auto BCins = Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns);
	BCins->ResultBattle_PostInit();
	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());
	   
	//SplayerController->SetSquadControllerInput(true);
	//Cast<USquadGameInstance>(gameIns)->SCMIns->EnableInput(SplayerController);
}