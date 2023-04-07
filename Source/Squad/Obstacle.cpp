// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempShape"));
	SetRootComponent(StaticComp);

	
	
	//StaticMesh'/Engine/BasicShapes/Cube.Cube'
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

