// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SquadGameInstance.h"
#include "PlayerSquadCharacter.h"
#include "SquadController.h"

void UBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();



	MoveButton = Cast<UButton>(GetWidgetFromName("MOVEBUTTON"));
	if (MoveButton != nullptr)
	{
		MoveButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickMoveButton);
	}
	AttackButton = Cast<UButton>(GetWidgetFromName("ATTACKBUTTON"));
	if (AttackButton != nullptr)
	{
		AttackButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickAttackButton);
	}
	TurnButton = Cast<UButton>(GetWidgetFromName("TURNBUTTON"));
	if (TurnButton != nullptr)
	{
		TurnButton->OnClicked.AddDynamic(this, &UBattleWidget::OnClickTurnButton);
	}


	TargetNameBlock = Cast<UTextBlock>(GetWidgetFromName("TARGETNAMEBLOCK"));
	if (TargetNameBlock != nullptr)
	{
		TargetNameBlock->SetText(FText::FromString(InitName));
	}
	CurrentCharacter = Cast<UTextBlock>(GetWidgetFromName("CURRENTCHARACTER"));
	if (CurrentCharacter != nullptr)
	{
		CurrentCharacter->SetText(FText::FromString(InitName));
	}
	WhosTurn = Cast<UTextBlock>(GetWidgetFromName("WHOSTURN"));
	if (WhosTurn != nullptr)
	{
		WhosTurn->SetText(FText::FromString(InitName));
	}


}


void UBattleWidget::OnClickMoveButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	auto Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == Cast<ASquadController>(Controller))
	{
		//Cast<ASquadController>(Controller)->SetCharacterAction(ECharAction::MOVE);
		if (GameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
			Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum = EStateEnum::SE_Move;
	}
	
}

void UBattleWidget::OnClickAttackButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	if (GameIns->SelectedCharacter != nullptr)
	{
		if (GameIns->SelectedCharacter == Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter) && Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_End)
		{
			UE_LOG(LogClass, Log, L"Already Shot");
		}
		else if (GameIns->SelectedCharacter == Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter) && Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Shot)
		{
			Cast<APlayerSquadCharacter>(GameIns->SelectedCharacter)->SetShotReady();
			UE_LOG(LogClass, Log, L"Ready to shot");
		}
		else
		{
			UE_LOG(LogClass, Log, L"Already Attack shape");
		}
	}
	else
	{
		UE_LOG(LogClass, Log, L" not Selected Character");
	}


}

void UBattleWidget::OnClickTurnButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (GameIns->BCIns != nullptr && GameIns->BCIns->IsBattleStart == true)
	{
		GameIns->BCIns->EndTurnSystem();
	}
}

///////////////////////////////////////////////////////////////////

void UBattleWidget::ChangeTargetName(FString NAME)
{
	TargetNameBlock->SetText(FText::FromString(NAME));
}

void UBattleWidget::ChangeCurrentCharName(FString NAME)
{
	CurrentCharacter->SetText(FText::FromString(NAME));
}

void UBattleWidget::ChangeWhosTurnName(bool NAME)
{
	if (!NAME)
	{
		WhosTurn->SetText(FText::FromString(L"My Turn"));
	}
	else
	{
		WhosTurn->SetText(FText::FromString(L"Enemy Turn"));
	}
}