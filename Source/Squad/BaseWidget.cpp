// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWidget.h"
#include "Components/Button.h"


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
	UE_LOG(LogClass, Log, L" MapButton");
}