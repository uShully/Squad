// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadController.h"
#include "Grid.h"
#include "SquadGameInstance.h"
#include "SquadGameMode.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/Engine.h"



ASquadController::ASquadController()
{
	bShowMouseCursor = true;

	Curve1 = nullptr;
	LerpTimelineLength = 3.0f;
	YOffset = 800.f;

	testbool = false;

}



void ASquadController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundHighlight;
	TArray<AActor*> FoundBattleSystem;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleController::StaticClass(), FoundBattleSystem);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACursorHighlight::StaticClass(), FoundHighlight);

	//Highlight = Cast<ACursorHighlight>(FoundHighlight[0]);
	if(FoundBattleSystem.Num() > 0)
	BattleController = Cast<ABattleController>(FoundBattleSystem[0]);

	SetInputMode(FInputModeGameAndUI());

	APawn* const MyPawn = GetPawn();
	//StartLoc = MyPawn->GetActorLocation();
	//EndLoc = FVector(StartLoc.X, StartLoc.Y + YOffset, StartLoc.Z);

	if (Curve1 != nullptr)
	{		
		// ������ Curve�� ����� callback �Լ���
		FOnTimelineFloat Curve1Callback;

		// FOnTimelineVector, FOnTimelineLinearColor ��� �߰��� ����

		// TimeLine�� ������ �� ������ callback �Լ���
		FOnTimelineEventStatic LerpTimelineFinishedCallBack;

		// callback �Լ��� ����� �Լ��� ���ε�
		// ���ε� �ϴ� �Լ����� UFUCNTION��ũ�ΰ� ����Ǿ���Ѵ�.
		Curve1Callback.BindUFunction(this, FName("TimelineCallbackTest"));
		LerpTimelineFinishedCallBack.BindUFunction(this, FName("TimelineFinishCallbackTest"));

		// Timeline�� Curve�� Curve�� ����� Callback �Լ��� �߰��մϴ�.
		LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);

		// Timeline�� ������ ȣ���� callback �Լ��� �߰��մϴ�.
		LerpTimeline.SetTimelineFinishedFunc(LerpTimelineFinishedCallBack);

		// Timeline�� ���̸� �����մϴ�.
		LerpTimeline.SetTimelineLength(LerpTimelineLength);
	}

	
}

// Ÿ�Ӷ��� ����� ����

void ASquadController::TimelineCallbackTest(float value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Curve Callback !! %f"), value));

	
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

	auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);
	auto pSelectedCharController = pSelectedChar->GetController();

	Cast<AAIController>(pSelectedCharController)->MoveToLocation(PathFinder());
}

void ASquadController::TimelineFinishCallbackTest()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, "Finish ");

	Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum = EStateEnum::SE_End;

	Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->EndTurnSystem();
}



//

FVector ASquadController::PathFinder()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);

	StartLocation = pSelectedChar->GetActorLocation();
	// EndLocation = ;
	FVector MiddleLocation(StartLocation.X, EndLocation.Y, StartLocation.Z);

	if (StartLoc.Y > MiddleLocation.Y)
	{
		if((pSelectedChar->GetActorLocation()).Y  < MiddleLocation.Y + 50.f)
		{
			
			return EndLocation;
		}
	
	}
	else if (StartLoc.Y < MiddleLocation.Y)
	{
		if ((pSelectedChar->GetActorLocation()).Y  > MiddleLocation.Y - 50.f)
		{
			return EndLocation;
		}
	}
	
	return MiddleLocation;
}

void ASquadController::SetHighLight(ACursorHighlight* Cursor)
{
	Highlight = Cursor;
}

void ASquadController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LerpTimeline.TickTimeline(DeltaTime);
	/*
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit == true)
	{
		
		if (Hit.Actor == Cast<ASquadCharacter>(Hit.Actor))
		{
			
			if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns != nullptr)
			{				
				FString Target = Hit.Actor->GetName();
				//Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->TargetName = Hit.Actor->GetName();
				Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeTargetName(Target);
			}
			// �׸��� �� �κ�
			//HitActorLoc = Hit.Actor->GetActorLocation() + (0.0f, 0.0f, 1.0f);
			//Highlight->SetActorLocation(HitActorLoc);
			//GetPawn()->SetActorLocation(Hit.Actor->GetActorLocation() + (0.0f, 0.0f, 1.0f));
			
		}
		else
		{
			

		}

		
	}	
	*/
	
}

void ASquadController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction(TEXT("Move"), EInputEvent::IE_Pressed, this, &ASquadController::MoveCharacter);
	InputComponent->BindAction(TEXT("Debug_Turn"), EInputEvent::IE_Pressed, this, &ASquadController::Debug_TurnSystem);
	InputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Pressed, this, &ASquadController::SetTargetCharacter);

	InputComponent->BindAction(TEXT("MoveCharacter"), EInputEvent::IE_Pressed, this, &ASquadController::MoveCharacter);
}

void ASquadController::MoveCharacter()
{
	FHitResult HitPos;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitPos);
	HitPos.ImpactPoint;

	
	if (HitPos.bBlockingHit)
	{
		//MoveToMouseCursor(HitPos.ImpactPoint);
	
		
		if (HitPos.Actor == Cast<AGrid>(HitPos.Actor))
		{
			auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());

			auto pSelectedChar = Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter);
			auto pGrid = Cast<AGrid>(HitPos.Actor);

		
			if (pSelectedChar->pGridOnCharacter != nullptr)
			{
				pSelectedChar->pGridOnCharacter->GridInfo.GOTO = EGridOntheObject::Normal;
				pSelectedChar->pGridOnCharacter->SetGridInfo_Material();
			}
			
			FVector SelectedCharacterLoc = pSelectedChar->GetActorLocation();
			FVector pGridLoc = pGrid->GetActorLocation();
			
			EndLocation = pGridLoc;

			StartLoc = pSelectedChar->GetActorLocation();
			
			LerpTimeline.PlayFromStart();

			pGrid->GridInfo.GOTO = EGridOntheObject::Player;
			pGrid->SetGridInfo_Material_temp2(); // 색변환 함수 임시
			pSelectedChar->pGridOnCharacter = pGrid;

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, " work  ");

		}
		else
		{
			
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, " Not ");




		}
		
	}	


}

void ASquadController::MoveToMouseCursor(const FVector Location)
{
	APawn* const MyPawn = GetPawn();

	FVector PawnLoc = MyPawn->GetActorLocation();
	FVector DesLoc(Location.X, Location.Y, 0.0f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DesLoc);

	
}



/*

void AGamePlayerController::HitYPosistion()
{
	FHitResult HitPos;
	GetHitResultUnderCursor(ECC_Visibility, false, HitPos);
	if (HitPos.bBlockingHit)
	{
		MoveToMouseCursor(HitPos.ImpactPoint);
		UE_LOG(LogClass, Log, TEXT("work "));
	}

	UE_LOG(LogClass, Log, TEXT("work2 "));
}

void AGamePlayerController::MoveToMouseCursor(const FVector Location)
{
	APawn* const MyPawn = GetPawn();

	FVector PawnLoc = MyPawn->GetActorLocation();
	FVector DesLoc(PawnLoc.X, Location.Y, 0.0f);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DesLoc);


}

*/

void ASquadController::Debug_TurnSystem()
{
	testbool = true;
}

void ASquadController::SetTargetCharacter()
{

	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitTarget);
	if (HitTarget.bBlockingHit == true && Cast<ABattleController>(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns)->WhosTurn == false)
	{
		if (HitTarget.Actor == Cast<ASquadCharacter>(HitTarget.Actor)) 
		{
			auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
			
			if (Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns != nullptr && Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true) // ��Ʋ��Ʈ�ѷ� ����� - ���� ���� ��
			{
				
				if(HitTarget.Actor == Cast<AEnemySquadCharacter>(HitTarget.Actor) && Cast<AEnemySquadCharacter>(HitTarget.Actor)->StateEnum != EStateEnum::SE_Death)
				{					
					gameIns->TargetCharacter = nullptr;
					gameIns->TargetCharacter = HitTarget.GetActor();

					
					FString Target = gameIns->TargetCharacter->GetName();
					Cast<ASquadGameMode>(GetWorld()->GetAuthGameMode())->ChangeTargetName(Target);

					if (gameIns->SelectedCharacter != nullptr && Cast<ASquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Shot)
					{
						Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->Debug_Shot(Cast<ASquadCharacter>(HitTarget.Actor));
						
						
						//Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetCharacterEnd();

						
						gameIns->BCIns->EndTurnSystem();
					}										
				}				
				if (HitTarget.Actor == Cast<APlayerSquadCharacter>(HitTarget.Actor) && Cast<APlayerSquadCharacter>(HitTarget.Actor)->StateEnum != EStateEnum::SE_Death)
				{
					
					if (gameIns->SelectedCharacter != nullptr && Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_Stay)
					{
						if(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
						{
							Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetStay();
						}
					}


					gameIns->BCIns->SetSelectedCharacter(Cast<ASquadCharacter>(HitTarget.Actor));
					
					Cast<APlayerSquadCharacter>(HitTarget.Actor)->PlaySelectedSound();

				}
				
				
				
			}
			
			//HitActorLoc = Hit.Actor->GetActorLocation() + (0.0f, 0.0f, 1.0f);
			//Highlight->SetActorLocation(HitActorLoc);
			//GetPawn()->SetActorLocation(Hit.Actor->GetActorLocation() + (0.0f, 0.0f, 1.0f));

		}
		else // �̵� �˰���� ���߿� �߰�
		{
			auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());
	
			// BC�� Ins�� ���� �ʱ�ȭ �ڵ� �ʿ�!!
			//
			if(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter) != nullptr)
			{ 
				if (HitTarget.Actor == Cast<AGrid>(HitTarget.Actor) &&
					Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum == EStateEnum::SE_Move)
				{
					MoveCharacter();
				}
			}
			else if(Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns != nullptr && Cast<USquadGameInstance>(GetWorld()->GetGameInstance())->BCIns->IsBattleStart == true)
			{
				if(gameIns->SelectedCharacter != nullptr)
				{
					if(Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->StateEnum != EStateEnum::SE_End)
					{
						Cast<APlayerSquadCharacter>(gameIns->SelectedCharacter)->SetStay();
					}
					// ����ó�� �ʿ��ҵ� �ݽ��� ����
					gameIns->BCIns->ClearSelectedCharacter();
				}
			}
		}
	}
}

// Ž�� ���� ĳ���� �̵� �Լ�

void ASquadController::CharacterMove()
{
	auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());


}

