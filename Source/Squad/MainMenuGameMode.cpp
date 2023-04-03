// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BeginplayWidget(TEXT("WidgetBlueprint'/Game/Map/M_MainMenu_Widget/M_MainMenu_WidgetBP.M_MainMenu_WidgetBP_C'"));
	if (BeginplayWidget.Succeeded())
	{
		BeginPlayWidgetClass = BeginplayWidget.Class;
	}
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	//ViewBeginPlayWidget();
}

void AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////

void AMainMenuGameMode::ViewBeginPlayWidget()
{
	ChangeMenuWidget(BeginPlayWidgetClass);
}