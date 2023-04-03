// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
		void beAfterLoadFinish();

};
