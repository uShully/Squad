// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "EventSpot.generated.h"

UCLASS()
class SQUAD_API AEventSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventSpot();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartBattleEvent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxColision;

	class ABattleTrigger* parentBattleTrigger;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool overlapSwitch;

	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> CrossRoadWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
public:	

};
