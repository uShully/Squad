// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplorerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSquadCharacter.h"
#include "SquadGameInstance.h"

// Sets default values
AExplorerController::AExplorerController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AExplorerController::BeginPlay()
{
	Super::BeginPlay();
	
	FindPlayerCharacter();

	InitPlayerCharacter();
}

// Called every frame
void AExplorerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplorerController::FindPlayerCharacter()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), PlayerCharacters);


}

void AExplorerController::InitPlayerCharacter()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		gameIns->CharSlot[i].CharacterInfo = Cast<APlayerSquadCharacter>(PlayerCharacters[i]);

		UE_LOG(LogClass, Log, L" Get in the Slot");
	}
}