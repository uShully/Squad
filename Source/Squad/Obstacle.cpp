// Fill out your copyright notice in the Description page of Project Settings.

#include "Obstacle.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempShape"));
	StaticComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Temp_Body(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Temp_Body.Succeeded())
	{
		StaticComp->SetStaticMesh(Temp_Body.Object);
	}
	
	
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

