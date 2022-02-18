// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UBattleWidget : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;


	UFUNCTION()
		void OnClickMoveButton();

	UFUNCTION()
		void OnClickAttackButton();

	UFUNCTION()
		void OnClickCoverButton();

	////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
		class UButton* AttackButton;

	//UPROPERTY(meta = (BindWidget))
	//	class UButton* MoveButton;
	   
	UPROPERTY(meta = (BindWidget))
		class UButton* CoverButton;


	//////////////////////////////////////

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



};
