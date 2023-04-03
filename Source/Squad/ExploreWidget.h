// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExploreWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UExploreWidget : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;

		UPROPERTY(meta = (BindWidget))
			class UTextBlock* TaskMarker;

		UPROPERTY(meta = (BindWidget))
			class UTextBlock* TaskText;

		UPROPERTY(meta = (BindWidget))
			class UTextBlock* TaskText2;

		UPROPERTY(meta = (BindWidget))
			class UImage* TaskBackGroundImage;

		UPROPERTY(meta = (BindWidget))
			class UImage* TaskBackGroundImage2;

public:
		UFUNCTION()
			void BeHideTaskCanvas();

private:

		int32 IsHideTaskCanvas = 0;
};
