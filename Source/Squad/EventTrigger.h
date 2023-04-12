// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EventTrigger.generated.h"

UCLASS()
class SQUAD_API AEventTrigger : public AActor
{
	GENERATED_BODY()
	

	// Sets default values for this actor's properties
	AEventTrigger();

	UFUNCTION()
	void InitEventBox();
	void CreateLastBattleStage();
	void CreateEventSpot();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	class UBoxComponent* BoxCollision;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage", meta = (AllowPrivateAccecce = "true"))
		int32 numberOfBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage", meta = (AllowPrivateAccecce = "true"))
		TSubclassOf<AActor> BattleTriggerToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage", meta = (AllowPrivateAccecce = "true"))
		TSubclassOf<AActor> EventSpotToSpawn;

private:

	TArray<AActor*> SpawnBox;


	
};
