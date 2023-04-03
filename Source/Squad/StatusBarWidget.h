// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Styling/SlateTypes.h"
#include "Engine/Texture2D.h"
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
	FORCEINLINE class UProgressBar *GetAmmoBar() { return AmmoBar; }

	virtual void NativeConstruct() override;
	void WidgetTest();

	void SetProgressBarImage(float HP);

	void SetBarRenderOpacity(float OpacityValue);

private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar *HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar *AmmoBar;
};
