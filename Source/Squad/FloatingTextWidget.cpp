// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingTextWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UFloatingTextWidget::NativeConstruct()
{
	Super::NativeConstruct();


	
	DisplayText->SetText(FText::FromString(FString::FromInt(TakenDamage)));
	
	PlayAnimation(FloatingAnim);
		
}

void UFloatingTextWidget::SetDisplayText(float Damage)
{
	
	this->TakenDamage = Damage;
}

