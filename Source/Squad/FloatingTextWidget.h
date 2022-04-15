// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "FloatingTextWidget.generated.h"

/**
 * 
 */

UCLASS()
class SQUAD_API UFloatingTextWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private: 	


//	class FWidgetAnimationDynamicEvent AnimationEvent;

	

	

public:

	UPROPERTY(Meta = (BindWidget), Meta = (AllowPrivateAccess = true))
	class UTextBlock* DisplayText;

	UPROPERTY(Meta = (BindWidgetAnim), Meta = (AllowPrivateAccess = true))
	class UWidgetAnimation* FloatingAnim;

	UPROPERTY(BlueprintReadOnly)
	float TakenDamage;

	UFUNCTION()
	void SetDisplayText(float Damage);

};
