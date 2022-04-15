// Fill out your copyright notice in the Description page of Project Settings.

#include "EventSpot.h"
#include "PlayerSquadCharacter.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "SquadAIController.h"
#include "BattleTrigger.h"
#include "SquadCharacter.h"
#include "EnemySquadCharacter.h"
#include "SquadAIController.h"
#include "Engine/Engine.h"

// Sets default values
AEventSpot::AEventSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxColision = CreateDefaultSubobject<UBoxComponent>(TEXT("EventSpot"));
	BoxColision->SetBoxExtent(FVector(24.f , 120.f , 24.f));
	BoxColision->SetCollisionProfileName("Trigger");

	RootComponent = BoxColision;

	BoxColision->OnComponentBeginOverlap.AddDynamic(this, &AEventSpot::OnOverlapBegin);



	overlapSwitch = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> CrossRoadWidget(L"WidgetBlueprint'/Game/DevFile/CrossRoadWidgetBP.CrossRoadWidgetBP_C'");
	if (CrossRoadWidget.Succeeded())
	{
		CrossRoadWidgetClass = CrossRoadWidget.Class;
	}
}

// Called when the game starts or when spawned
void AEventSpot::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEventSpot::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Cast<APlayerSquadCharacter>(OtherActor))
	{
		if (!overlapSwitch) {


			StartBattleEvent();

			overlapSwitch = true;
		}
	}
}

void AEventSpot::StartBattleEvent()
{
	if(parentBattleTrigger != nullptr)
	{
		parentBattleTrigger->StartBattleEvent();
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(false);

	}
	else
	{
		ChangeMenuWidget(CrossRoadWidgetClass);
		// 갈림길 이벤트 창
		auto gameIns = GetWorld()->GetGameInstance();
		Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(true);
		Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement();
	}
}

void AEventSpot::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
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