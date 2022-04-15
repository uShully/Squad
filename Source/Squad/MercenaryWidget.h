// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonStruct.h"
#include "MercenaryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UMercenaryWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	
	//// 임시 작성 ////

	UPROPERTY(meta = (BindWidget))
	class UButton* IncludeFirstMercenary;

	UPROPERTY(meta = (BindWidget))
	class UButton* IncludeSecondMercenary;

	UPROPERTY(meta = (BindWidget))
	class UButton* IncludeThirdMercenary;


	UPROPERTY(meta = (BindWidget))
	class UButton* ExcludeFirstMercenary;

	// 변경 예정 //
	FCharacterDataStruct FirstMercenaryData;
	FCharacterDataStruct SecondMercenaryData;
	FCharacterDataStruct ThirdMercenaryData;
	
	///////////////

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_0;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_1;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_2;

	TArray<UImage*> ImageArray;

	///////////////

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ImageText_0;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ImageText_1;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ImageText_2;
	

	TArray<UTextBlock*> ImageTextArray;

	///////////////

	UFUNCTION()
	void ClickInCludeFirstMercenary();

	UFUNCTION()
	void ClickInCludeSecondMercenary();

	UFUNCTION()
	void ClickInCludeThirdMercenary();
	
	UFUNCTION()
	void ClickExCludeFirstMercenary();

	UFUNCTION()
	void RefreshMark_Include(FText ClassName);

	UFUNCTION()
	void RefreshMark_Exclude();
};
