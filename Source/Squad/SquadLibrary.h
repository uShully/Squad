// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SquadLibrary.generated.h"

UENUM(BlueprintType)
enum ELocalizationCulture
{
	EN,
	KO,
	AR,
	BG,
	DA,
	NL,
	FI,
	FR,
	DE,
	ID,
	IT,
	LA,
	MS,
	NO,
	PT_BR,
	PT,
	RU,
	ES,
	SV,
	TR,
	VI,
	JA_JP,
	ZH_HANS,
	ZH_HANT
};

/**
 * 
 */
UCLASS()
class SQUAD_API USquadLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, Category = Translation)
		static void ChangeLocalizationCulture(ELocalizationCulture culture);

	
};

