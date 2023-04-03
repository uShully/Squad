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
#include "SquadCharacterSkillComponent.h"
#include "BattleWidget.h"
#include "SquadCameraManager.h"
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

	static ConstructorHelpers::FClassFinder<UUserWidget> CrossRoadWidget(L"WidgetBlueprint'/Game/UI/CrossRoadWidgetBP.CrossRoadWidgetBP_C'");
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
	if (OtherActor == Cast<APlayerSquadCharacter>(OtherActor))	{
		if (!overlapSwitch) {
			StartBattleEvent();
			overlapSwitch = true;
		}
	}
}

void AEventSpot::StartBattleEvent()
{
	if(parentBattleTrigger != nullptr)	{
		RemoteCooldown();
		Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->ControlCharacterCameraMovement(false);
		if (parentBattleTrigger->BTState == EBattleTriggerState::Normal)
			parentBattleTrigger->StartBattleEvent();
		else
			parentBattleTrigger->StartBattleEvent_Boss();
	}
	else {
		ChangeMenuWidget(CrossRoadWidgetClass);
		// 갈림길 이벤트 창
		auto gameIns = GetWorld()->GetGameInstance();
		Cast<USquadGameInstance>(gameIns)->SCMIns->Control_PlayerCharacterMovement(true);
		Cast<USquadGameInstance>(gameIns)->SCMIns->Control_CameraMovement(true);
	}
}

void AEventSpot::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{	
	if (CurrentWidget != nullptr)	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)	{
			CurrentWidget->AddToViewport();
		}
	}
}

void AEventSpot::RemoteCooldown()
{
	auto BC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns;
	auto SC = Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->SCMIns;
	auto FrindlyCharacterList = SC->FriendlyCharList;

	for (int32 i = 0; i < FrindlyCharacterList.Num(); i++) {
		if (Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->Character_Skill1_Cooldown > 0) {
			Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->Character_Skill1_Cooldown -= 1 ;
		
		}
	

		if (Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->Character_Skill2_Cooldown > 0) {
			Cast<APlayerSquadCharacter>(FrindlyCharacterList[i])->Character_Skill2_Cooldown -= 1 ;
			
		}
	}
	Cast<UBattleWidget>(Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->GetCurrentWidget())->UpdateCharacterSkillMarker(Cast<APlayerSquadCharacter>(BC->GetSelectedCharacter()));

	
}