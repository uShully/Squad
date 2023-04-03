// Fill out your copyright notice in the Description page of Project Settings.

#include "ExploreWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine//Engine.h"

void UExploreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TaskMarker = Cast<UTextBlock>(GetWidgetFromName("TASKMARKER"));
	TaskText = Cast<UTextBlock>(GetWidgetFromName("TASKTEXT"));
	TaskText2 = Cast<UTextBlock>(GetWidgetFromName("TASKTEXT2"));
	TaskBackGroundImage = Cast<UImage>(GetWidgetFromName("TASKBACKGROUNDIMAGE"));
	TaskBackGroundImage2 = Cast<UImage>(GetWidgetFromName("TASKBACKGROUNDIMAGE2"));
	
}

void UExploreWidget::BeHideTaskCanvas()
{
	
		
	if (IsHideTaskCanvas == 0)
	{
		
		TaskMarker->SetVisibility(ESlateVisibility::Hidden);
		TaskText->SetVisibility(ESlateVisibility::Hidden);
		TaskText2->SetVisibility(ESlateVisibility::Hidden);
		TaskBackGroundImage->SetVisibility(ESlateVisibility::Hidden);
		TaskBackGroundImage2->SetVisibility(ESlateVisibility::Hidden);

		IsHideTaskCanvas = 1;
	}
	else if(IsHideTaskCanvas == 1)
	{
		
		TaskMarker->SetVisibility(ESlateVisibility::Visible);
		TaskText->SetVisibility(ESlateVisibility::Visible);
		TaskText2->SetVisibility(ESlateVisibility::Visible);
		TaskBackGroundImage->SetVisibility(ESlateVisibility::Visible);
		TaskBackGroundImage2->SetVisibility(ESlateVisibility::Visible);

		IsHideTaskCanvas = 0;
	}
}