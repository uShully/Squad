// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SQUAD_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	AMainMenuGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
		TSubclassOf<UUserWidget> BeginPlayWidgetClass;

protected:

	// 초기 시작 위젯
	

	UPROPERTY()
	UUserWidget* CurrentWidget;

	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	void ViewBeginPlayWidget();
	
};
