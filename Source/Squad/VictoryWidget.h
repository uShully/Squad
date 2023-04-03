// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UVictoryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickResultButton();

	//////////////////////////
	
public:

	UPROPERTY(BlueprintReadOnly ,meta = (BindWidget))
	class UButton* ResultButton;

	////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultTextBlock;


};
