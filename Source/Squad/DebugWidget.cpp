// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SquadGameInstance.h"
#include "PlayerSquadCharacter.h"
#include "SquadController.h"
	
void UDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();


	SetButton = Cast<UButton>(GetWidgetFromName("SETBUTTON"));
	if (SetButton != nullptr)
	{
		SetButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickSetButton);
	}
	MoveButton = Cast<UButton>(GetWidgetFromName("MOVEBUTTON"));
	if (MoveButton != nullptr)
	{
		MoveButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickMoveButton);
	}
	AttackButton = Cast<UButton>(GetWidgetFromName("ATTACKBUTTON"));
	if (AttackButton != nullptr)
	{
		AttackButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickAttackButton);
	}
	TurnButton = Cast<UButton>(GetWidgetFromName("TURNBUTTON"));
	if (TurnButton != nullptr)
	{
		TurnButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickTurnButton);
	}

	HeelButton = Cast<UButton>(GetWidgetFromName("HEELBUTTON"));
	if (HeelButton != nullptr)
	{
		HeelButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickHeelButton);
	}
	SetDebugButton = Cast<UButton>(GetWidgetFromName("SETDEBUGBUTTON"));
	if (SetDebugButton != nullptr)
	{
		SetDebugButton->OnClicked.AddDynamic(this, &UDebugWidget::OnClickDebugButton);
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



void UDebugWidget::OnClickSetButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (GameIns->BCIns != nullptr)
	{
		GameIns->BCIns->StartTurnSystem();
	}

}

void UDebugWidget::OnClickMoveButton()
{
	/*
	auto Controller = GetWorld()->GetFirstPlayerController();
	if (Controller == Cast<ASquadController>(Controller))
	{
		//Cast<ASquadController>(Controller)->SetCharacterAction(ECharAction::MOVE);
		
	}
	*/
}

void UDebugWidget::OnClickAttackButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	if(GameIns->SelectedCharacter != nullptr)
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

void UDebugWidget::OnClickTurnButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (GameIns->BCIns != nullptr && GameIns->BCIns->GetIsBattleStart() == true)
	{
		GameIns->BCIns->EndTurnSystem();
	}
}

void UDebugWidget::ChangeTargetName(FString NAME)
{
	TargetNameBlock->SetText(FText::FromString(NAME));
}

void UDebugWidget::ChangeCurrentCharName(FString NAME)
{
	CurrentCharacter->SetText(FText::FromString(NAME));
}

void UDebugWidget::ChangeWhosTurnName(bool NAME)
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





void UDebugWidget::OnClickHeelButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (GameIns->BCIns != nullptr && GameIns->BCIns->GetIsBattleStart() == true && GameIns->TargetCharacter != nullptr)
	{
		Cast<ASquadCharacter>(GameIns->TargetCharacter)->LifePoint = Cast<ASquadCharacter>(GameIns->TargetCharacter)->MaxLifePoint;
	}
}

void UDebugWidget::OnClickDebugButton()
{
	auto GameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	if (GameIns->BCIns != nullptr && GameIns->BCIns->SetDebug == false)
	{
		GameIns->BCIns->SetDebug = true;
	}
	else if (GameIns->BCIns != nullptr && GameIns->BCIns->SetDebug == true)
	{
		GameIns->BCIns->SetDebug = false;
	}
}