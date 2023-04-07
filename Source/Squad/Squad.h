// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Squad, Log, All);
#define SGLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define SGLOG_S(Verbosity) UE_LOG(Squad, Verbosity, TEXT("%s"), *SGLOG_CALLINFO)
#define SGLOG(Verbosity, Format, ...) UE_LOG(Squad, Verbosity, TEXT("%s%s"), *SGLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define SGCHECK(Expr, ...) { if(!(Expr)) {SGLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}
