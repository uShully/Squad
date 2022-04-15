// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectThreadContext.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "FeedbackContext.h"
#include "AssetTable.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API UAssetTable : public UObject
{
	GENERATED_BODY()
	
};

#define DATATABLE_SAFEMODE 0

USTRUCT(BlueprintType)
struct FAssetRefTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftObjectPtr<UObject> AssetRef;
};

struct FDataTableUtil
{
	static FString GetAssetPath(const FString& DataTablePath, const FName& RowName)
	{
#if DATATABLE_SAFEMODE == 0
		// check is called in constructor. if not assertion.
		FUObjectThreadContext& ThreadContext = FUObjectThreadContext::Get();
		verifyf(ThreadContext.IsInConstructor > 0, TEXT("Attempt to call outside of constructor."));

#else

#endif
	}
};