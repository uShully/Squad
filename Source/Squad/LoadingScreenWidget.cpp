// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadingScreenWidget.h"
#include "SquadGameMode.h"

void ULoadingScreenWidget::beAfterLoadFinish()
{
	auto SquadGameIns = Cast<ASquadGameMode>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode()));
	SquadGameIns->GetFriendlyChar();
}