// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	
	UFUNCTION()
	void OnClickSetButton();

	UFUNCTION()
	void OnClickMoveButton();

	UFUNCTION()
	void OnClickAttackButton();
	
	UFUNCTION()
	void OnClickTurnButton();
	
	// 디버그용 버튼

	UFUNCTION()
	void OnClickHeelButton();

	UFUNCTION()
	void OnClickDebugButton();
			

	
	UPROPERTY(meta = (BindWidget))
	class UButton* SetButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* AttackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MoveButton;

	// 디버그용 버튼


	UPROPERTY(meta = (BindWidget))
	class UButton* TurnButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HeelButton;

	

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TargetNameBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentCharacter;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WhosTurn;

public:

	void ChangeTargetName(FString NAME);
	void ChangeCurrentCharName(FString NAME);
	void ChangeWhosTurnName(bool NAME);


	FString InitName = TEXT("NULL");

	UPROPERTY(Blueprintreadwrite, meta = (BindWidget))
	class UButton* SetDebugButton;

};
