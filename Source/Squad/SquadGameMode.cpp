// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SquadGameMode.h"
#include "SquadCameraManager.h"
#include "SquadController.h"
#include "DebugWidget.h"
#include "BattleWidget.h"
#include "BattleTrigger.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "SquadGameInstance.h"

ASquadGameMode::ASquadGameMode()
{
	// set default pawn class to our Blueprinted character

	FString path = L"";
	path = L"Blueprint'/Game/DevFile/SquadCameraManager.SquadCameraManager_C'";

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(*path);
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = NULL;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BattleStartWidget(L"WidgetBlueprint'/Game/DevFile/Widget/BattleWidget.BattleWidget_C'");
	if (BattleStartWidget.Succeeded())
	{
		BattleStartWidgetClass = BattleStartWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DecisionWidget(L"WidgetBlueprint'/Game/DevFile/Widget/DecisionWidget.DecisionWidget_C'");
	if (DecisionWidget.Succeeded())
	{
		DecisionWidgetClass = DecisionWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> VictoryWidget(L"WidgetBlueprint'/Game/DevFile/VictoryWidgetBP.VictoryWidgetBP_C'");
	if (VictoryWidget.Succeeded())
	{
		VictoryWidgetClass = VictoryWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DefeatWidget(L"WidgetBlueprint'/Game/DevFile/DefeatWidgetBP.DefeatWidgetBP_C'");
	if (DefeatWidget.Succeeded())
	{
		DefeatWidgetClass = DefeatWidget.Class;
	}

}


// 

void ASquadGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASquadGameMode::ViewDecisionWidget()
{
	ChangeMenuWidget(DecisionWidgetClass);
}

void ASquadGameMode::ViewBattleWidget()
{
	ChangeMenuWidget(BattleStartWidgetClass);
}

void ASquadGameMode::ViewVictoryWidget()
{
	ChangeMenuWidget(VictoryWidgetClass);
}

void ASquadGameMode::ViewDefeatWidget()
{
	ChangeMenuWidget(DefeatWidgetClass);
}

TSubclassOf<UUserWidget> ASquadGameMode::GetVictoryWidgetClass()
{
	return VictoryWidgetClass;
}

TSubclassOf<UUserWidget> ASquadGameMode::GetDefeatWidgetClass()
{
	return DefeatWidgetClass;
}


void ASquadGameMode::StartBattle()
{
	Cast<ABattleTrigger>(BTIns)->InitBattleSetting();
	
	BTIns = nullptr;
	/*
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameIns->BCIns->InitBattleSetting();
	*/
}

void ASquadGameMode::EndBattle()
{
	CurrentWidget->RemoveFromViewport();
	CurrentWidget = nullptr;
}

void ASquadGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}	
}

void ASquadGameMode::ChangeTargetName(FString Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeTargetName(Name);
}

void ASquadGameMode::ChangeCurrentCharName(FString Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeCurrentCharName(Name);
}

void ASquadGameMode::ChangeWhosTurnName(bool Name)
{
	Cast<UBattleWidget>(CurrentWidget)->ChangeWhosTurnName(Name);
}