// Fill out your copyright notice in the Description page of Project Settings.

#include "StatusBarWidget.h"



void UStatusBarWidget::WidgetTest()
{
	
	
	
}

void UStatusBarWidget::SetProgressBarImage(float HP)
{
	FProgressBarStyle HealthbarStyle;

	FSlateBrush a;
	
	//	Texture2D'/Game/skillnum2.skillnum2'
	if(HP == 2)
	{
		a.SetResourceObject(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("Texture2D'/Game/skillnum2.skillnum2'")));
	}
	else if (HP == 1)
	{
		a.SetResourceObject(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("Texture2D'/Game/skillnum1.skillnum1'")));
	}
	HealthbarStyle.SetBackgroundImage(a);
	HealthbarStyle.SetFillImage(a);
	//HealthbarStyle.SetBackgroundImage()
	//Cast<FSlateBrush>(StaticLoadObject(FSlateBrush::StaticClass(), NULL, ContentPath));
	HealthBar->WidgetStyle = HealthbarStyle;
}

void UStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//WidgetTest();
	
}

void UStatusBarWidget::SetBarRenderOpacity(float OpacityValue)
{
	HealthBar->SetRenderOpacity(OpacityValue);
	AmmoBar->SetRenderOpacity(OpacityValue);
}