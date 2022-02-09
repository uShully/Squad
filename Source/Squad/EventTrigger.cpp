// Fill out your copyright notice in the Description page of Project Settings.

#include "EventTrigger.h"

// Sets default values
AEventTrigger::AEventTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EventCollision"));
	BoxCollision->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	BoxCollision->SetCollisionProfileName("EventTrigger");


	numberOfBox = 0;
}

// Called when the game starts or when spawned
void AEventTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	InitEventBox();
}

// Called every frame
void AEventTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTrigger::InitEventBox()
{
	BoxCollision->SetBoxExtent(FVector(1350.f * numberOfBox, 900.f, 600.f));
	
	FActorSpawnParameters SpawnParams;
		
	for (int i = 0; i < numberOfBox; i++)
	{
		FVector Loc(this->GetActorLocation().X - i * 2700.f , this->GetActorLocation().Y , this->GetActorLocation().Z);

		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(BattleTriggerToSpawn, Loc, this->GetActorRotation(), SpawnParams);
		SpawnBox.Add(SpawnedActorRef);

		auto ActorLoc = SpawnedActorRef->GetActorLocation();
		SpawnedActorRef->SetActorLocation(ActorLoc + FVector(-1320.f, 0.f, 0.f));
	}

	auto ETLoc = GetActorLocation();
	SetActorLocation(ETLoc + FVector(-1320.f * numberOfBox, 0.f, 0.f));

}