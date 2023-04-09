// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWidget.h"
#include "Components/Button.h"
#include "SquadGameInstance.h"
#include "Engine/Engine.h"


void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CommissionButton = Cast<UButton>(GetWidgetFromName("COMMISSIONBUTTON"));
	if (CommissionButton != nullptr)
	{
		CommissionButton->OnClicked.AddDynamic(this, &UBaseWidget::OnClickCommissionButton);
	}

	MercenaryButton = Cast<UButton>(GetWidgetFromName("MERCENARYBUTTON"));
	if (MercenaryButton != nullptr)
	{
		MercenaryButton->OnClicked.AddDynamic(this, &UBaseWidget::OnClickMercenaryButton);
	}

	MapButton = Cast<UButton>(GetWidgetFromName("MAPBUTTON"));
	if(MapButton != nullptr)
	{
		MapButton->OnClicked.AddDynamic(this, &UBaseWidget::OnClickMapButton);
	}

	gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());	
	gameIns->CharSlotEmpty();
}

void UBaseWidget::CharacterSetting(float ClassNumber)
{
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
			
	if (gameIns->CharSlot.MaxSlotNum > gameIns->GetCharacterDataNum()) // 과거 문제 지점
	{
		tempData.ClassNumber = ClassNumber;
		tempData.IsExist = true;
		gameIns->IncludeCharacterData(tempData);
		
	}
}

void UBaseWidget::OnClickCommissionButton()
{
	UE_LOG(LogClass, Log, L" CommissionButton");
}

void UBaseWidget::OnClickMercenaryButton()
{
	UE_LOG(LogClass, Log, L" MercenaryButton");
}

void UBaseWidget::OnClickMapButton()
{
	CharacterSetting(5.f);
	CharacterSetting(0.f);
	CharacterSetting(3.f);
	CharacterSetting(8.f);
	
	USquadGameInstance* gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());


	gameIns->GameDifficulty = 0;

	UE_LOG(LogClass, Log, L" MapButton");
}