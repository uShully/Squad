// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE class UProgressBar *GetHealthBar() { return HealthBar; }

	void WidgetTest();

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;

	
};
