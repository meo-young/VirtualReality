// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(AVRProject, Verbose, All);

#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define LOG(Format, ...) UE_LOG(AVRProject, Warning, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__));