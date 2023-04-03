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
	
public:	
	// Sets default values for this actor's properties
	AEventTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stage")
	class UBoxComponent* BoxCollision;

	UFUNCTION()
	void InitEventBox();

	void CreateLastBattleStage();

	void CreateEventSpot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	int32 numberOfBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	TSubclassOf<AActor> BattleTriggerToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
		TSubclassOf<AActor> EventSpotToSpawn;


	
	
private:

	TArray<AActor*> SpawnBox;

};
