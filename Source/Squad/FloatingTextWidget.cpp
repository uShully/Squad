// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingTextWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UFloatingTextWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if (TakenDamage > 0)
		DisplayText->SetText(FText::FromString(FString::FromInt(TakenDamage)));
	else if (TakenDamage <= 0)
		DisplayText->SetText(FText::FromString(TEXT("Miss")));

	PlayAnimation(FloatingAnim);
		
}

void UFloatingTextWidget::SetDisplayText(float Damage)
{
	
	this->TakenDamage = Damage;
}

