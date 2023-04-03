// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "CrossroadWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UCrossroadWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
		

	UFUNCTION()
	void OnClickEvent1();

	UFUNCTION()
	void OnClickEvent2();

	UFUNCTION()
		void OnClickEvent3();

	void SetLevel_NextLevel(int32 ButtonNum);

	void RecoverUnit(int32 Value);

	void ChangeGamedifiiculty(int32 Vaule);


	FString Control_MapData();

	void SaveCharacterData();

	//////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UButton* UpCrossroad;

	UPROPERTY(meta = (BindWidget))
	class UButton* MiddleCrossroad;

	UPROPERTY(meta = (BindWidget))
		class UButton* DownCrossroad;

	/////////////////////////////////////////////

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CrossroadName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HomeName;

	FString CrossRoadStringName = TEXT("CrossRoad");
	FString HomeStringName = TEXT("Home");
};
