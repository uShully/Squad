// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Obstacle.generated.h"

UCLASS()
class SQUAD_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shape", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticComp;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coordinate")
		int32 XPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coordinate")
		int32 YPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector ObstacleCollectionValue;

};
