// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DecisionWidget.h"
#include "DecisionResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UDecisionResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
		virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmationButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Eventname;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Event;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* OutputValue;

	UFUNCTION()
		void SetResultText();

	UFUNCTION()
	void OnClickConfirmationButton();

public:

	EDecisionEventResultEnum DecisionResultEnum;
	int32 MainStream;
	int32 SubStream;
	int32 SubStreamSuccFail;
	FText EventResultText;
	FText EventResultValueText;
	//float stateValue;
};
