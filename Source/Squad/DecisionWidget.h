// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DecisionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UDecisionWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickEvent1();

	UFUNCTION()
	void OnClickEvent2();

	//////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UButton* Event1;

	UPROPERTY(meta = (BindWidget))
		class UButton* Event2;

	/////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event1Name;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event2Name;
	

	FString InitName = TEXT("Fight");
};
