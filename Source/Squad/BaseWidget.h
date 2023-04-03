// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonStruct.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
		void CharacterSetting(float ClassNumber);

	UFUNCTION()
		void OnClickCommissionButton();

	UFUNCTION()
		void OnClickMercenaryButton();

	UFUNCTION()
		void OnClickMapButton();

	//////

	UPROPERTY(meta = (BindWidget))
		class UButton* CommissionButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MercenaryButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MapButton;

	///////

	FCharacterDataStruct tempData;

};
