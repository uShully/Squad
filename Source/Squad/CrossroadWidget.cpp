// Fill out your copyright notice in the Description page of Project Settings.

#include "CrossroadWidget.h"
#include "SquadGameMode.h"
#include "BattleTrigger.h"
#include "BattleController.h"
#include "SquadGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "SquadController.h"
#include "SquadCameraManager.h"

void UCrossroadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UpCrossroad != nullptr)
	{
		UpCrossroad->OnClicked.AddDynamic(this, &UCrossroadWidget::OnClickEvent1);
	}
	if (MiddleCrossroad != nullptr)
	{
		MiddleCrossroad->OnClicked.AddDynamic(this, &UCrossroadWidget::OnClickEvent2);
	}
	if (DownCrossroad != nullptr)
	{
		DownCrossroad->OnClicked.AddDynamic(this, &UCrossroadWidget::OnClickEvent3);
	}

	auto SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());

	SplayerController->SetSquadControllerInput(false);
	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns->DisableInput(SplayerController);

	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->SaveCharacterData();

}

void UCrossroadWidget::OnClickEvent1() 
{
	SetLevel_NextLevel(1);

}

void UCrossroadWidget::OnClickEvent2() 
{	
	SetLevel_NextLevel(2);
}

void UCrossroadWidget::OnClickEvent3()
{
	SetLevel_NextLevel(3);
}

void UCrossroadWidget::SetLevel_NextLevel(int32 ButtonNum)
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
	switch (ButtonNum)
	{
		case 1:
			break;
		case 2:
			RecoverUnit(5);
			ChangeGamedifiiculty(1);
			break;
		case 3:
			RecoverUnit(10);
			ChangeGamedifiiculty(2);
			break;
		default:
			break;
	}
	
	Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->SaveCharacterData();
	
	auto MapName = Control_MapData();
	if(MapName != TEXT("END"))
	UGameplayStatics::OpenLevel(gameIns, *MapName);
}

void UCrossroadWidget::RecoverUnit(int32 Value)
{
	TArray<AActor*> Charlist = Cast<ASquadCameraManager>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns)->FriendlyCharList;
 	for (int32 i = 0; i < Charlist.Num(); i++)
	{
		Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint += Value;
		if (Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint > Cast<APlayerSquadCharacter>(Charlist[i])->MaxLifePoint)
			Cast<APlayerSquadCharacter>(Charlist[i])->LifePoint = Cast<APlayerSquadCharacter>(Charlist[i])->MaxLifePoint;
	}
	
}

void UCrossroadWidget::ChangeGamedifiiculty(int32 Vaule)
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	gameIns->GameDifficulty = gameIns->GameDifficulty + Vaule;
}

FString UCrossroadWidget::Control_MapData()
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	FString ReturnMapName;

	if (gameIns->Mission1_mapData.Num() > 1) {
		gameIns->Mission1_mapData.RemoveAt(0);
		
		switch(gameIns->Mission1_mapData[0]) {
			case 1:
				return TEXT("M_PlayLevel1");				
				break;
			case 2:
				return TEXT("M_PlayLevel1_2");
				break;
			case 3:
				return TEXT("M_PlayLevel1_3");
				break;	
		}		
	}

	return TEXT("END");
}

void UCrossroadWidget::SaveCharacterData()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto BC = gameIns->BCIns;
	auto SC = gameIns->SCMIns;
	auto FrindlyCharacterList = SC->FriendlyCharList;

	gameIns->CharSlot.EmptyCharacterDataArry();
	//gameIns->CharSlot.CharacterDataArry.Add(FrindlyCharacterList[0]);
}