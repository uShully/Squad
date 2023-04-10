// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "PlayerSquadCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SquadGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SquadGameMode.h"
#include "SquadAIController.h"
#include "SquadController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ASquadCameraManager::ASquadCameraManager()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteLocation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false;

	UnitPos_First = CreateDefaultSubobject<UBoxComponent>(TEXT("UnitPos_First"));
	UnitPos_First->SetBoxExtent(FVector(32.f, 32.f, 5.f));
	UnitPos_First->SetCollisionProfileName("UnitPos_First");
	UnitPos_First->SetupAttachment(RootComponent);

	UnitPos_Last = CreateDefaultSubobject<UBoxComponent>(TEXT("UnitPos_Last"));
	UnitPos_Last->SetBoxExtent(FVector(32.f, 32.f, 5.f));
	UnitPos_Last->SetCollisionProfileName("UnitPos_Last");
	UnitPos_Last->SetupAttachment(RootComponent);

	UnitPos_First->OnComponentBeginOverlap.AddDynamic(this, &ASquadCameraManager::OnOverlapBegin);
	UnitPos_Last->OnComponentBeginOverlap.AddDynamic(this, &ASquadCameraManager::OnOverlapBegin);

}

void ASquadCameraManager::BeginPlay()
{
	Super::BeginPlay();

	ExploreRot = FRotator(ExplorePitch, ExploreYaw, ExploreRoll);  // 5 -50
 	BattleRot = FRotator(BattlePitch, BattleYaw, BattleRoll); // pitch , yaw , roll

	auto gameIns = GetWorld()->GetGameInstance();
	Cast<USquadGameInstance>(gameIns)->SCMIns = this;

	SplayerController = Cast<ASquadController>(GetWorld()->GetFirstPlayerController());

	SplayerController->SetSquadControllerInput(false);
	DisableInput(SplayerController);

	UnitPos_Check_First = false;
	UnitPos_Check_Last = false;
	UnitPos_Init = false;

	UnitPos_First_RelativeLocation = UnitPos_First->RelativeLocation;
	UnitPos_Last_RelativeLocation = UnitPos_Last->RelativeLocation;

	
}

void ASquadCameraManager::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == Cast<APlayerSquadCharacter>(OtherActor)) {

		if (OverlappedComp == UnitPos_First && Cast<APlayerSquadCharacter>(OtherActor)->numbering == 0 && UnitPos_Check_First == false) {
			UnitPos_Check_First = true;
			UnitPos_First_RelativeLocation = UnitPos_First->RelativeLocation;
		
		}

		auto gameIns = Cast<USquadGameInstance>(GetWorld()->GetGameInstance());		
		int32 NumofChar = FriendlyCharList.Num();//(gameIns->CharSlot.SlotNum);

		if (OverlappedComp == UnitPos_Last && Cast<APlayerSquadCharacter>(OtherActor)->numbering == NumofChar - 1 && UnitPos_Check_Last == false) {
			UnitPos_Check_Last = true;
			UnitPos_Last_RelativeLocation = UnitPos_Last->RelativeLocation;
		
		}
	}

	if (UnitPos_Check_First == true && UnitPos_Check_Last == true && UnitPos_Init == false) {
			UnitPos_Init = true;
		
			FTimerHandle WaitHandle;			
			
			auto gameIns = GetWorld()->GetGameInstance();			
		
			SplayerController->SetBattleController();
			SplayerController->SetSelectedCharacterInfo();
			SplayerController->Target_Explorer = Cast<APlayerSquadCharacter>(Cast<USquadGameInstance>(gameIns)->BCIns->GetSelectedCharacter());

			ControlValue_PlayerCharacterMovement_BeginPlay = false;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					SplayerController->SetSquadControllerInput(true);
					EnableInput(SplayerController);

				}), 1.5f, false);
	}


		// 일단은 임시 코드 || 문제는 1명, 2명, 3명일때 작동하지않는다 반드시 보완이 필요
		// - UnitPos_Last를 캐릭터 인원수에 맞춰서 Loc을 변경시켜 해결 
}

void ASquadCameraManager::SetUnitPos_Last_Location(int32 number)
{
	// 4명 Y 650 , 3명 Y 650 - (180 * 1)
	// n명 Y 650 - (180 * (4 - n) )

	FVector CompLoc = UnitPos_First->RelativeLocation;
	FVector tempVector(0.f, 180 * ( number - 1), 0.f);
	FVector ChangeCompLoc = CompLoc + tempVector;

	UnitPos_Last->SetRelativeLocation(ChangeCompLoc);
}

void ASquadCameraManager::EventSpotOverlapLocation_Save()
{
	EventSpotOverlapLocation_First = UnitPos_First->GetComponentLocation();
	EventSpotOverlapLocation_Last = UnitPos_Last->GetComponentLocation();
}

void ASquadCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeArmLeght(DeltaTime);
	Control_SetBattleCameraLocation(DeltaTime);
}

void ASquadCameraManager::GetFriendlyChar()
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerSquadCharacter::StaticClass(), FriendlyCharList);
	SortFrindlyCharList();
}


void ASquadCameraManager::SortFrindlyCharList()
{

	for (int32 i = 0 ; i < FriendlyCharList.Num(); i++)
	{
		for (int32 j = 0; j < FriendlyCharList.Num(); j++)
		{

			if (Cast<APlayerSquadCharacter>(FriendlyCharList[j])->BattleLineNumber == i)
			{
				if (i == j)
				{
					Cast<APlayerSquadCharacter>(FriendlyCharList[j])->numbering = i;
				}
				else
				{
					Cast<APlayerSquadCharacter>(FriendlyCharList[j])->numbering = i;
					FriendlyCharList.Swap(j, i);
				}				
			}
		}	
	}
}

void ASquadCameraManager::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ASquadCameraManager::MoveRight);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASquadCameraManager::MoveChar);

	
#ifdef __SGDEBUG__

	PlayerInputComponent->BindAction("Etc", EInputEvent::IE_Released, this, &ASquadCameraManager::zoomswitch);
	PlayerInputComponent->BindAction("Plus", EInputEvent::IE_Released, this, &ASquadCameraManager::PlusTargetArmLeght);
	PlayerInputComponent->BindAction("Minus", EInputEvent::IE_Released, this, &ASquadCameraManager::MinusTargetArmLength);
	PlayerInputComponent->BindAction("PlusRot", EInputEvent::IE_Released, this, &ASquadCameraManager::PlusCameraBoomRotator);
	PlayerInputComponent->BindAction("MinusRot", EInputEvent::IE_Released, this, &ASquadCameraManager::MinusCameraBoomRotator);

#endif // DEBUGMODE_ONOFF

}

void ASquadCameraManager::MoveRight(float Value)
{
	if(!ControlValue_CameraMovement)
	AddMovementInput(FVector(-1.f,  0.f, 0.f), Value);
	
}

void ASquadCameraManager::MoveChar(float Value)
{
	if (ControlValue_PlayerCharacterMovement && !ControlValue_PlayerCharacterMovement_BeginPlay) { // cvp = true , cvpb = false
		for (int32 i = 0; i < FriendlyCharList.Num(); i++)
		{
			Cast<ASquadCharacter>(FriendlyCharList[i])->MoveRight(Value);
		}
	}
	else {

	}
}

void ASquadCameraManager::Control_PlayerCharacterMovement(bool Switch)
{
	// true - 캐릭터가 카메라에 따라 움직임, false - 캐릭터가 카메라 이동에 움직이지 않음
	if (Switch == true)
		ControlValue_PlayerCharacterMovement = true;
	else
		ControlValue_PlayerCharacterMovement = false;
}

void ASquadCameraManager::Control_CameraMovement(bool Switch)
{
	// true - 카메라 이동가능 , false - 카메라가 이동하지 않음
	if (Switch == false)
		ControlValue_CameraMovement = false;
	else
		ControlValue_CameraMovement = true;
}

void ASquadCameraManager::Control_SetBattleInit(FVector Loc)
{
	FVector tempVector(0.f, 0.f, 50.f);
	BattleLocation = (Loc + tempVector);
}

void ASquadCameraManager::Control_SetExploreInit(FVector Loc)
{
	ExplorerLocation = Loc;
}

void ASquadCameraManager::Control_SetBattleCameraLocation(float DeltaTime)
{		
	if(MoveSwitch == true)
	{
		if(IsExploreToBattle == true)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector Interp = FMath::VInterpTo(CurrentLocation, BattleLocation , GetWorld()->GetDeltaSeconds(), 5.0f);
			
			SetActorLocation(Interp);
	
		

			if (CurrentLocation.X < BattleLocation.X + 10.f)
			{
				MoveSwitch = false;
				IsExploreToBattle = false;
			}
		}
		else if (IsBattleToExplore == true)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector Interp = FMath::VInterpTo(CurrentLocation, ExplorerLocation, GetWorld()->GetDeltaSeconds(), 2.0f);

			SetActorLocation(Interp);



			if (CurrentLocation.X > ExplorerLocation.X - 90.f)
			{
				MoveSwitch = false;
				IsBattleToExplore = false;
				UnitPos_First->SetRelativeLocation(UnitPos_First_RelativeLocation);
				//UnitPos_Last->SetRelativeLocation(UnitPos_Last_RelativeLocation);
				//ControlValue_PlayerCharacterMovement_BeginPlay = false;
			}
		}
	}


	
}

void ASquadCameraManager::SetUnitMovement_Delay(int32 num)
{
	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			Control_PlayerCharacterMovement(true);
			Control_CameraMovement(false);

		}), num, false);
}

// 디버그용 기능

void ASquadCameraManager::ChangeArmLeght(float DeltaTime)
{
	if (ZoomBool)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, MaxTargetArmLength, DeltaTime, 4.0f);
		FRotator Interp = FMath::RInterpTo(CameraBoom->RelativeRotation, BattleRot, DeltaTime, 4.0f);
		CameraBoom->SetRelativeRotation(Interp);
	}
	else
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, minTargetArmLength, DeltaTime, 4.0f);
		FRotator Interp = FMath::RInterpTo(CameraBoom->RelativeRotation, ExploreRot, DeltaTime, 4.0f);
		CameraBoom->SetRelativeRotation(Interp);
	}
}



void ASquadCameraManager::zoomswitch()
{
	if (!ZoomBool)
	{		
		ZoomBool = true;
	}
	else
	{	
		ZoomBool = false;
	}

}

void ASquadCameraManager::PlusTargetArmLeght()
{
	CameraBoom->TargetArmLength += 100.f;
}

void ASquadCameraManager::MinusTargetArmLength()
{
	CameraBoom->TargetArmLength -= 100.f;
}

void ASquadCameraManager::PlusCameraBoomRotator()
{
	FRotator Rot = CameraBoom->GetComponentRotation();
	CameraBoom->AddRelativeRotation(FRotator(-1.f, 0.f, 0.f));

}

void ASquadCameraManager::MinusCameraBoomRotator()
{
	FRotator Rot = CameraBoom->GetComponentRotation();
	CameraBoom->AddRelativeRotation(FRotator(1.f, 0.f, 0.f));
}

